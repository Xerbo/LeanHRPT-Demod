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

#ifndef DSP_SYMBOL_SYNC_H
#define DSP_SYMBOL_SYNC_H

#include "fir_intrepolator.h"
#include "block.h"

#include <complex>
#include <cstring>
#include <vector>
#include <iostream>

#define MIN_SAMPLE_HISTORY 3
#define HISTORY_SIZE 3

class SymbolSync : public Block<complex, complex> {
    public:
        SymbolSync(size_t order, float sps, std::pair<float, float> loop, float max_dev = 0.0f)
            : d_sps(sps),
              d_target_sps(sps),
              d_alpha(loop.first),
              d_beta(loop.second),
              d_limit(max_dev),
              d_order(order),
              d_history(HISTORY_SIZE) { };

        size_t work(const std::complex<float> *in, std::complex<float> *output, size_t length) {
            // Manage the history
            size_t totalLength = sampleHistory + length;
            if (ring_buffer.size() < totalLength) {
                ring_buffer.resize(totalLength);
            }

            memcpy(&ring_buffer[sampleHistory], in, length * sizeof(std::complex<float>));

            size_t symbols = internal_work(ring_buffer.data(), output, totalLength);

            sampleHistory = std::max((int)totalLength - (int)consumed, MIN_SAMPLE_HISTORY);

            memmove(&ring_buffer[0], &ring_buffer[totalLength - sampleHistory], (sampleHistory) * sizeof(std::complex<float>));

            return symbols;
        }
    private:
        float d_sps;
        const float d_target_sps;
        float d_mu = 0.5f;
        const float d_alpha; // gainMu (phase)
        const float d_beta; // gainOmega (frequency)
        const float d_limit; // omegaRelativeLimit
        const size_t d_order;

        FIRInterpolator d_interp;
        std::vector<std::complex<float>> d_history;

        size_t internal_work(const std::complex<float> *in, std::complex<float> *out, size_t n) {
            size_t ii = 0; // Input index
            size_t oo = 0; // Output index
            //n -= d_interp.ntaps();
            size_t n2 = n - d_interp.ntaps() - 16;

            while (ii < n2) {
                // Get the next symbol and add it to the history
                out[oo] = d_interp.interpolate(&in[ii], d_mu);
                d_history.erase(d_history.begin()); // Delete the first element
                d_history.push_back(out[oo]); // Add to back

                // Mueller and Muller TED
                float error;
                switch (d_order) { 
                    case 2: 
                        error = slicer(d_history[1].real()) * d_history[2].real() -
                                slicer(d_history[2].real()) * d_history[1].real();
                        break;
                    case 4:
                        error = (slicer(d_history[1].real()) * d_history[2].real() -
                                 slicer(d_history[2].real()) * d_history[1].real()) +
                                (slicer(d_history[1].imag()) * d_history[2].imag() -
                                 slicer(d_history[2].imag()) * d_history[1].imag());
                        break;
                    default:
                        throw std::runtime_error("Invalid or unsupported PSK order");
                }

                // Adjust sps (frequency)
                d_sps = d_sps + d_beta*error;
                d_sps = d_target_sps + clamp(d_sps - d_target_sps, d_limit);

                // Adjust Mu (phase)
                d_mu = d_mu + d_sps + d_alpha*error;
                ii += (size_t)floor(d_mu); // Shift input index
                d_mu -= floor(d_mu); // Wrap to [0,1]

                oo++;
            }

            consumed = ii;
            if (consumed > n) {
                std::cerr << "Consumed more samples than input." << std::endl;
            }
            return oo;
        }

        float slicer(float symbol) {
            return symbol < 0.0f ? -1.0f : 1.0f;
        }

        std::vector<std::complex<float>> ring_buffer;
        size_t sampleHistory = MIN_SAMPLE_HISTORY;
        size_t consumed;
};

#endif
