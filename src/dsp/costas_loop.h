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

#ifndef DSP_COSTAS_LOOP_H
#define DSP_COSTAS_LOOP_H

#include "util/math.h"
#include "block.h"
#include <cmath>
#include <complex>

static inline float phase_detector_qpsk(std::complex<float> sample) {
    return ((sample.real() > 0 ? 1.0f : -1.0f) * sample.imag() -
            (sample.imag() > 0 ? 1.0f : -1.0f) * sample.real());
}

static inline float phase_detector_bpsk(std::complex<float> sample) {
    return (sample.real() * sample.imag());
}

// Locks onto a carrier with a PLL and outputs the input signal mixed with the carrier
class CostasLoop : public Block<complex, complex> {
    public:
        CostasLoop(size_t order, float alpha, float beta, float max_freq = M_PIf32)
            : d_order(order),
              d_alpha(alpha),
              d_beta(beta),
              d_max_freq(max_freq) { }
        CostasLoop(size_t order, std::pair<float, float> loop, float max_freq = M_PIf32)
            : CostasLoop(order, loop.first, loop.second, max_freq) { }

        size_t work(const std::complex<float> *in, std::complex<float> *out, size_t n) {
            for (size_t i = 0; i < n; i++) {
                // Mix incoming signal with the locked oscillator
                out[i] = in[i] * std::complex<float>(fast_cos(d_phase), -fast_sin(d_phase));

                // Remove DC offset
                accumulator = out[i]*0.001f + accumulator*0.999f;
                out[i] -= accumulator;

                // Calculate phase error
                float error;
                switch (d_order) { 
                    case 2: error = phase_detector_bpsk(out[i]); break;
                    case 4: error = phase_detector_qpsk(out[i]); break;
                    default: throw std::runtime_error("Invalid of unsupported PSK order");
                }
                //error = clamp(error, 1.0f);

                // Adjust frequency based on the phase error (clamping if necessary)
                d_freq = clamp(d_freq + d_beta * error, d_max_freq);
                d_phase = d_phase + d_freq + d_alpha * error;

                // Could also use std::remainder here
                // But is faster due to the miniscule changes in d_phase
                if (d_phase >  M_PIf32) d_phase -= 2*M_PIf32;
                if (d_phase < -M_PIf32) d_phase += 2*M_PIf32;
            }

            return n;
        }
    private:
        const size_t d_order;
        const float d_alpha;
        const float d_beta;
        const float d_max_freq;

        float d_freq = 0.0f;
        float d_phase = 0.0f;
        std::complex<float> accumulator;
};

#endif
