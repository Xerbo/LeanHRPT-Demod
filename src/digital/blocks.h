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
        FengyunViterbi() : iVit(0.45f, 5, true), qVit(0.45f, 5, true) { }

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
        Viterbi iVit;
        Viterbi qVit;
        FengyunDiff diff;

        std::vector<std::complex<int8_t>> iSymbols;
        std::vector<std::complex<int8_t>> qSymbols;
        std::vector<uint8_t> iData;
        std::vector<uint8_t> qData;

        bool offset = false;
};

class VCDUExtractor : public Block<uint8_t, uint8_t> {
    public:
        VCDUExtractor(bool cadu = false) : Block(1024), use_cadu(cadu) { }

        size_t work(const uint8_t *in, uint8_t *out, size_t n) {
            uint8_t frame[1024];
            if (deframer.work(in, frame, n)) {
                derand.work(frame, 1024);
                rs.decode_intreleaved_ccsds(frame);

                uint8_t VCID = frame[5] & 0x3f;
                if (VCID != 63) {
                    if (use_cadu) {
                        std::memcpy(out, frame, 1024);
                        return 1024;
                    } else {
                        std::memcpy(out, &frame[4], 892);
                        return 892;
                    }
                }
            }

            return 0;
        }
    private:
        const bool use_cadu;
        ccsds::Deframer deframer;
        ccsds::Derand derand;
        SatHelper::ReedSolomon rs;
};

#endif
