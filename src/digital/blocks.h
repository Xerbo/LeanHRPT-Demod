/*
 * LeanHRPT Demod
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

#ifndef DIGITAL_BLOCKS_H
#define DIGITAL_BLOCKS_H

#include "dsp/block.h"
#include "util/math.h"

#include "deframer.h"
#include "derand.h"
#include "reedsolomon.h"
#include "viterbi.h"
#include "diff.h"
#include "correlator.h"
#include "packetfixer.h"

const std::vector<uint64_t> CCSDS_QPSK_SYNC = {
    0xfca2b63db00d9794,
    0x56fbd394daa4c1c2,
    0x035d49c24ff2686b,
    0xa9042c6b255b3e3d,
    0xfc51793e700e6b68,
    0xa9f7e368e558c2c1,
    0x03ae86c18ff19497,
    0x56081c971aa73d3e
};
const std::vector<uint64_t> CCSDS_BPSK_SYNC = {
    0xfca2b63db00d9794,
    0x035d49c24ff2686b
};

class CCSDSCorrelator : public Block<complex, uint8_t> {
    public:
        CCSDSCorrelator(size_t frame_size = 16384, std::vector<uint64_t> syncwords = CCSDS_QPSK_SYNC) : d_frame_size(frame_size) {
            for (const auto &syncword : syncwords) {
                correlator.addWord(syncword);
            }

            frame.resize(frame_size);
            decoded.resize(frame_size/16);
            d_conv = correct_convolutional_sse_create(2, 7, new uint16_t[2]{0x4F, 0x6D});
        }
        ~CCSDSCorrelator() {
            correct_convolutional_sse_destroy(d_conv);
        }

        size_t work(const complex *in, uint8_t *out, size_t n) {
            bool have_frame = false;
            size_t nsymbols;

            switch (correlator.getCorrelationWordCount()) {
                case 2: // BPSK
                    nsymbols = n;
                    deinterleaved.reserve(nsymbols+64);

                    for (size_t i = 0; i < 64; i++) deinterleaved[i] = wraparound[i];
                    for (size_t i = 0; i < n; i++) {
                        deinterleaved[i] = map_symbol(in[i].real());
                    }
                    for (size_t i = 0; i < 64; i++) wraparound[i] = deinterleaved[nsymbols + i];
                    break;
                case 8: // QPSK
                    nsymbols = n*2;
                    deinterleaved.reserve(nsymbols+64);

                    for (size_t i = 0; i < 64; i++) deinterleaved[i] = wraparound[i];
                    for (size_t i = 0; i < n; i++) {
                        deinterleaved[64 + i * 2 + 0] = map_symbol(in[i].real());
                        deinterleaved[64 + i * 2 + 1] = map_symbol(in[i].imag());
                    }
                    for (size_t i = 0; i < 64; i++) wraparound[i] = deinterleaved[nsymbols + i];
                    break;
                default:
                    throw std::runtime_error("CCSDSCorrelator: Unknown modulation");
            }

            if (frame_pos != 0) {
                size_t symbols_needed = d_frame_size - frame_pos;
                for (size_t i = 0; i < std::min(symbols_needed, nsymbols); i++) {
                    frame[frame_pos++] = deinterleaved[i];
                }

                if (frame_pos == d_frame_size) {
                    frame_pos = 0;

                    fixer.fixPacket(frame.data(), d_frame_size, shift, invert);
                    correct_convolutional_sse_decode_soft(d_conv, frame.data(), d_frame_size, decoded.data());
                    derand.work(decoded.data(), d_frame_size/16);
                    std::vector<ssize_t> errors = rs.decode_intreleaved(decoded.data(), false);

                    uint8_t VCID = decoded[5] & 0x3f;
                    if (VCID != 63 && (errors[0] != -1 || errors[1] != -1 || errors[2] != -1 || errors[3] != -1)) {
                        decoded[0] = 0x1A;
                        decoded[1] = 0xCF;
                        decoded[2] = 0xFC;
                        decoded[3] = 0x1D;
                        memcpy(out, decoded.data(), d_frame_size/16);
                        have_frame = true;
                    }
                }
            }

            correlator.correlate(deinterleaved.data(), nsymbols+64);
            uint32_t highest_correlation = correlator.getHighestCorrelation();
            uint32_t word = correlator.getCorrelationWordNumber();
            uint32_t pos = correlator.getHighestCorrelationPosition();

            if (frame_pos == 0 && highest_correlation > 10) {
                switch (correlator.getCorrelationWordCount()) {
                    case 2: // BPSK
                        invert = word > 1;
                        shift = SatHelper::PhaseShift::DEG_0;
                        break;
                    case 8: // QPSK
                        invert = word > 3;
                        shift = (SatHelper::PhaseShift)(word % 4);
                        break;
                    default:
                        throw std::runtime_error("CCSDSCorrelator: Unknown modulation");
                }

                for (size_t i = pos; i < nsymbols; i++) {
                    frame[frame_pos++] = deinterleaved[i];
                }
            }

            return have_frame ? d_frame_size/16 : 0;
        }

    private:
        const size_t d_frame_size;
        std::vector<uint8_t> deinterleaved;
        std::vector<uint8_t> frame;
        std::vector<uint8_t> decoded;
        uint8_t wraparound[64];

        SatHelper::Correlator correlator;
        SatHelper::PacketFixer fixer;
        correct_convolutional_sse *d_conv;
        SatHelper::ReedSolomon rs;
        ccsds::Derand derand;

        size_t frame_pos = 0;
        bool invert;
        SatHelper::PhaseShift shift;

        uint8_t map_symbol(float x) {
            uint8_t y = clamp(x*50.0f + 127.0f, 0, UINT8_MAX);
            if (y == 128) y = 127;
            return y;
        }
};

class MetopViterbi : public Block<complex, uint8_t> {
    public:
        MetopViterbi() : vit(0.45f, 5) { }

        size_t work(const complex *in, uint8_t *out, size_t n) {
            symbols.reserve(n);
            for (size_t i = 0; i < n; i++) {
                float real = clamp(in[i].real()*127.0f, 127.0f);
                float imag = clamp(in[i].imag()*127.0f, 127.0f);
                symbols[i] = std::complex<int8_t>(real, imag);
            }

            return vit.work(symbols.data(), out, BUFFER_SIZE);
        }
    private:
        Viterbi vit;
        std::vector<std::complex<int8_t>> symbols;
};

class FengyunViterbi : public Block<complex, uint8_t> {
    public:
        FengyunViterbi(bool fy3c_mode = false) : _fy3c_mode(fy3c_mode), iVit(0.45f, 5, true), qVit(0.45f, 5, true) { }

        size_t work(const complex *in, uint8_t *out, size_t n) {
            iSymbols.reserve(n/2);
            qSymbols.reserve(n/2);
            iData.reserve(n/2); // Way larger than needed
            qData.reserve(n/2); // Way larger than needed

            // Deinterleave I and Q (badly)
            for (size_t i = 0; i < n/2-2; i++) {
                iSymbols[i] = std::complex<int8_t>(
                    clamp(in[i*2     + offset].imag()*127.0f, 127.0f),
                    clamp(in[i*2 + 1 + offset].imag()*127.0f, 127.0f)
                );
                qSymbols[i] = std::complex<int8_t>(
                    clamp(in[i*2     + offset].real()*127.0f, 127.0f), 
                    clamp(in[i*2 + 1 + offset].real()*127.0f, 127.0f)
                );
                if (_fy3c_mode) {
                    qSymbols[i] = -qSymbols[i];
                    std::swap(qSymbols[i], iSymbols[i]);
                }
            }

            size_t iBytes, qBytes;
            #pragma omp parallel sections num_threads(2)
            {
                #pragma omp section
                {
                    iBytes = iVit.work(iSymbols.data(), iData.data(), n/2);
                }
                #pragma omp section
                {
                    qBytes = qVit.work(qSymbols.data(), qData.data(), n/2);
                }
            }

            if (iBytes && qBytes) {
                diff.work(qData.data(), iData.data(), iBytes, out);
                return iBytes+qBytes;
            } else {
                offset = !offset;
            }

            return 0;
        }
    private:
        bool _fy3c_mode;
        Viterbi iVit;
        Viterbi qVit;
        FengyunDiff diff;

        std::vector<std::complex<int8_t>> iSymbols;
        std::vector<std::complex<int8_t>> qSymbols;
        std::vector<uint8_t> iData;
        std::vector<uint8_t> qData;

        bool offset = false;
};

class Fengyun3CViterbi : public FengyunViterbi {
    public:
        Fengyun3CViterbi() : FengyunViterbi(true) {}
};

class VCDUExtractor : public Block<uint8_t, uint8_t> {
    public:
        VCDUExtractor(std::string extension, bool ccsds = true) : Block(1024), use_cadu(extension != "vcdu"), d_ccsds(ccsds) { }

        size_t work(const uint8_t *in, uint8_t *out, size_t n) {
            uint8_t frame[1024];
            if (deframer.work(in, frame, n)) {
                derand.work(frame, 1024);
                rs.decode_intreleaved(frame, d_ccsds);

                if (use_cadu) {
                    std::memcpy(out, frame, 1024);
                    return 1024;
                } else {
                    uint8_t VCID = frame[5] & 0x3f;
                    if (VCID != 63) {
                        std::memcpy(out, &frame[4], 892);
                        return 892;
                    }
                }
            }

            return 0;
        }
    private:
        const bool use_cadu;
        const bool d_ccsds;
        ccsds::Deframer deframer;
        ccsds::Derand derand;
        SatHelper::ReedSolomon rs;
};

#endif
