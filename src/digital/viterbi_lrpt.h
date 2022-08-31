/*
 * LeanHRPT Viterbi
 * Copyright (C) 2021 Xerbo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LEANHRPT_LRPTVITERBI_H
#define LEANHRPT_LRPTVITERBI_H

#include "viterbi.h"

// r=1/2 k=7 viterbi with automatic phase/shift detection
class LrptViterbi {
    public:
        LrptViterbi(float ber_threshold, size_t outsync_after);
        ~LrptViterbi();

        size_t work(std::complex<int8_t> *in, uint8_t *out, size_t len);
        ViterbiState get_state() {
            return d_state;
        };
    private:
        float d_ber_threshold;
        size_t d_outsync_after;

        // Probability look up table
        uint8_t prob_table[256];
        void init_prob_table();
        uint8_t prob_calc(int8_t in);

        float ber_calc(std::complex<uint8_t> *insymbols, size_t len);

        void rotate_phase(bool rotate, size_t len, std::complex<uint8_t> *in, std::complex<uint8_t> *out);
        void depuncture(std::complex<uint8_t> *in, uint8_t *out, size_t len);

        void enter_state(ViterbiState state);

        correct_convolutional_sse *d_conv;
        correct_convolutional_sse *d_conv_ber_test;

        size_t d_shift;
        size_t d_phase;

        size_t d_invalid_buffers;
        ViterbiState d_state;

        std::vector<std::complex<uint8_t>> input_symbols;
        std::vector<std::complex<uint8_t>> input_symbols_rotated;

        std::vector<uint8_t> depunctured_symbols;
        std::vector<uint8_t> decoded_data;
        std::vector<uint8_t> encoded_data;
};

#endif
