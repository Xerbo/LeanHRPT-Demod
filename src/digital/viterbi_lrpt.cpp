/*
 * Copyright 2008 Free Software Foundation, Inc.
 * Copyright 2021 Xerbo
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR Ar version 3, or (at your option)
 * any later version. PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "viterbi_lrpt.h"

#include <cstring>

// Uncomment this if you want debug information
//#define DEBUG

// Number of symbols to test BER against
#define BER_TEST_SYMS 1024

#ifndef M_E
#define M_E 2.7182818284590452354
#endif
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

LrptViterbi::LrptViterbi(float ber_threshold, size_t outsync_after)
    : d_ber_threshold(ber_threshold),
      d_outsync_after(outsync_after) {
    enter_state(Syncing);
    init_prob_table();

    d_conv_ber_test = correct_convolutional_sse_create(2, 7, new uint16_t[2]{0x4F, 0x6D});
    d_conv          = correct_convolutional_sse_create(2, 7, new uint16_t[2]{0x4F, 0x6D});

    d_phase = 0;
    d_shift = 0;
}

LrptViterbi::~LrptViterbi() {
    correct_convolutional_sse_destroy(d_conv);
    correct_convolutional_sse_destroy(d_conv_ber_test);
}

void LrptViterbi::enter_state(ViterbiState state) {
    switch (state) {
        case Syncing:
            d_state = Syncing;
            break;
        case Synced:
            d_state = Synced;
            d_invalid_buffers = 0;
            break;
        default:
            throw std::runtime_error("Viterbi: invalid state");
    }
}

// r=3/4 depuncture
void LrptViterbi::depuncture(std::complex<uint8_t> *in, uint8_t *out, size_t len) {
    size_t bits = 0;
    for (size_t i = 0; i < len; i++) {
        out[bits++] = in[i].imag();
        out[bits++] = in[i].real();
    }
}

// Gets a bit...
template <typename T>
inline bool getBit(T data, int bit) {
    return (data >> bit) & 1;
}

// Calculate BER
float LrptViterbi::ber_calc(std::complex<uint8_t> *insymbols, size_t len) {
    depunctured_symbols.reserve(len * 2);
    decoded_data.reserve(len); // Far bigger than it needs to be
    encoded_data.reserve(len * 2);

    // Decode
    depuncture(insymbols, depunctured_symbols.data(), len);
    correct_convolutional_sse_decode_soft(d_conv_ber_test, depunctured_symbols.data(), len*2, decoded_data.data());

    // Reencode
    correct_convolutional_sse_encode(d_conv_ber_test, decoded_data.data(), len/8, encoded_data.data());

    // Calculate the number of differences between the input and reencoded data
    size_t errors = 0;
    for (size_t k = 0; k < len*2; k++) {
        if ((depunctured_symbols[k] > 127) != getBit(encoded_data[k / 8], 7 - k%8)) {
            errors++;
        }
    }

    // Calculate BER
    return (float)errors / (float)(len*2) * 8.0f;
}

// Rotate phase
void LrptViterbi::rotate_phase(bool rotate, size_t len, std::complex<uint8_t> *in, std::complex<uint8_t> *out) {
    if (rotate) {
        for (size_t i = 0; i < len; i++) {
            out[i].real(in[i].imag());
            out[i].imag(UINT8_MAX - in[i].real());
        }
    } else {
        std::memcpy(out, in, len * sizeof(std::complex<uint8_t>));
    }
}

// Calculate the probability that a symbol is a one
// See http://www.dsplog.com/2007/08/05/bit-error-probability-for-bpsk-modulation/
// and https://www.desmos.com/calculator/2dd0myeckw
void LrptViterbi::init_prob_table() {
    for (int i = -128; i < 128; i++) {
        float E = 0.707f;
        float N = 0.32f;

        float val = i/127.0f;

        float a = 1.0f/std::sqrt((float)M_PI*N) * std::pow((float)M_E, -std::pow(-val + std::sqrt(E), 2.0f)/N);
        float b = 1.0f/std::sqrt((float)M_PI*N) * std::pow((float)M_E, -std::pow( val + std::sqrt(E), 2.0f)/N);

        prob_table[i+128] = (a - b)*127.0f + 127.0f;
    }
}
uint8_t LrptViterbi::prob_calc(int8_t in) {
    return prob_table[in+128];
}

// Main work function
size_t LrptViterbi::work(std::complex<int8_t> *in_syms, uint8_t *out, size_t len) {
    input_symbols.reserve(len);
    input_symbols_rotated.reserve(len);

    for (size_t i = 0; i < len; i++) {
        input_symbols[i] = std::complex<uint8_t>(
            prob_calc(in_syms[i].real()),
            prob_calc(in_syms[i].imag())
        );

        // 128 is used for a punctured symbol, which these aren't
        if (input_symbols[i].real() == 128) input_symbols[i].real(127);
        if (input_symbols[i].imag() == 128) input_symbols[i].imag(127);
    }

    switch (d_state) {
        case Syncing:{
            float bestber = 1;
            for (int rotation = 0; rotation < 2; rotation++) {
                rotate_phase(rotation == 1, BER_TEST_SYMS, input_symbols.data(), input_symbols_rotated.data());

                for (int shift = 0; shift < 2; shift++) {
                    float ber = ber_calc(input_symbols_rotated.data() + shift, BER_TEST_SYMS);
                    if (ber < bestber) {
                        d_phase = rotation;
                        d_shift = shift;
                        bestber = ber;
                    }
                }
            }
            #ifdef DEBUG
            printf("Viterbi: Syncing: %s, %s, BER = %f\n", d_phase?"rotated 90deg":"no rotation  ", d_shift?"shifted ":"no shift", bestber);
            #endif
            if (bestber < d_ber_threshold) {
                #ifdef DEBUG
                printf("Viterbi: switch state to Synced\n");
                #endif
                enter_state(Synced);
            }
            break;
        }
        case Synced: {
            rotate_phase(d_phase == 1, BER_TEST_SYMS, input_symbols.data(), input_symbols_rotated.data());
            float ber = ber_calc(input_symbols_rotated.data() + d_shift, BER_TEST_SYMS);

            if (ber > d_ber_threshold) {
                #ifdef DEBUG
                printf("Viterbi: Synced: BER %3f exceeds BER threshold, %zu/%zu\n", ber, d_invalid_buffers, d_outsync_after);
                #endif
                d_invalid_buffers++;

                if (d_invalid_buffers > d_outsync_after) {
                    #ifdef DEBUG
                    printf("Viterbi: switch state to Syncing\n");
                    #endif
                    enter_state(Syncing);
                }
            } else {
                d_invalid_buffers = 0;
            }
            break;
        }
        default:
            throw std::runtime_error("Viterbi: bad state\n");
    }

    // Just exit now if we aren't synced yet
    if (d_state == Syncing)
        return 0;

    // Actually decode the data
    depunctured_symbols.reserve(len * 2);

    rotate_phase(d_phase == 1, len, input_symbols.data(), input_symbols_rotated.data());
    depuncture(input_symbols_rotated.data() + d_shift, depunctured_symbols.data(), len - d_shift);
    correct_convolutional_sse_decode_soft(d_conv, depunctured_symbols.data(), len*2, out);

    return len/8;
}
