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

//class SymbolSync {
//    public:
//        SymbolSync(float sps, std::pair<float, float> loop, float max_dev) { };
//};

class ClockRecovery : public Block<complex, complex> {
    public:
        ClockRecovery(float omega, float mu = 0.5f, float gain_mu = 0.1f, bool qpsk = true) 
            : d_omega(omega),
              d_mu(mu),
              d_gain_mu(gain_mu),
              d_qpsk(qpsk),
              d_history(3) { }

        size_t work(const std::complex<float> *in, std::complex<float> *output, size_t length) {
            // Manage the history
            size_t totalLength = sampleHistory + length;
            if (ring_buffer.size() < totalLength) {
                ring_buffer.resize(totalLength);
            }

            memcpy(&ring_buffer[sampleHistory], in, length * sizeof(std::complex<float>));

            size_t symbols = internal_work(ring_buffer.data(), output, totalLength);

            sampleHistory = std::max(totalLength - consumed, (size_t)MIN_SAMPLE_HISTORY);

            memmove(&ring_buffer[0], &ring_buffer[totalLength - sampleHistory], (sampleHistory) * sizeof(std::complex<float>));

            return symbols;
        }
    private:
        float d_omega;
        float d_mu = 0.5;
        const float d_gain_mu = 0.175;
        bool d_qpsk;

        FIRInterpolator interp;
        std::vector<std::complex<float>> d_history;
        std::vector<std::complex<float>> ring_buffer;
        size_t sampleHistory = MIN_SAMPLE_HISTORY;
        size_t consumed = 0;

        float slicer(float symbol) {
            return symbol > 0 ? 1.0f : 0.0f;
        }

        int internal_work(const std::complex<float> *in, std::complex<float> *out, size_t inputLen) {
            size_t inputIndex = 0;
            size_t outputIndex = 0;
            size_t numberInput = inputLen - 8;

            while (inputIndex < numberInput) {
                // Get the next symbol and manage history
                out[outputIndex] = interp.interpolate(&in[inputIndex], d_mu);
                d_history.pop_back();
                d_history.insert(d_history.begin(), out[outputIndex]);

                // Zero crossing TED
                float ted_error;
                if (d_qpsk) {
                    ted_error = (slicer(d_history[2].real()) - slicer(d_history[0].real())) * d_history[1].real() +
                                (slicer(d_history[2].imag()) - slicer(d_history[0].imag())) * d_history[1].imag();
                } else {
                    ted_error = (slicer(d_history[2].real()) - slicer(d_history[0].real())) * d_history[1].real();
                }

                // Adjust Mu (PI filter)
                d_mu = d_mu + d_omega + d_gain_mu*ted_error;
                inputIndex += (size_t)floor(d_mu); // Shift input index
                d_mu -= floor(d_mu); // Wrap to [0,1]

                outputIndex++;
            }

            consumed = inputIndex;

            if (consumed > inputLen) {
                std::cerr << "Consumed more samples than input." << std::endl;
            }

            return outputIndex;
        }
};

#endif
