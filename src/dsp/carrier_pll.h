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

#ifndef DSP_CARRIER_PLL_H
#define DSP_CARRIER_PLL_H

#include "block.h"
#include "util/sse.h"
#include "util/math.h"
#include <cmath>
#include <vector>
#include <complex>

// Locks onto a carrier with a PLL and outputs the input signal mixed with the carrier
class CarrierPLL : public Block<complex, complex> {
    public:
        CarrierPLL(float alpha, float beta, float max_freq = M_PIf32)
            : d_alpha(alpha),
              d_beta(beta),
              d_max_freq(max_freq) { }
        CarrierPLL(std::pair<float, float> loop, float max_freq = M_PIf32)
            : CarrierPLL(loop.first, loop.second, max_freq) { }

        size_t work(const std::complex<float> *in, std::complex<float> *out, size_t n) {
            phases.reserve(n);
            for (size_t i = 0; i < n/4; i++) {
                sse::complex samp = sse::deinterleave_complex(&in[i*4]);
                __m128 phase = sse::atan2(samp.imag, samp.real);
                _mm_storeu_ps(&phases[i*4], phase);
            }

            for (size_t i = 0; i < n; i++) {
                // Mix incoming signal with the locked oscillator
                out[i] = in[i] * std::complex<float>(std::cos(d_phase), -std::sin(d_phase));

                // Calculate phase error (wrapping if necessary)
                float phase = std::isnan(phases[i]) ? std::arg(in[i]) : phases[i];
                float error = std::remainder(phase - d_phase, M_PIf32*2.0f);

                // Adjust frequency based on the phase error (clamping if necessary)
                d_freq = d_freq + d_beta * error;
                d_phase = d_phase + d_freq + d_alpha * error;

                // Could also use std::remainder here
                // But is faster due to the miniscule changes in d_phase
                while (d_phase >  M_PIf32) d_phase -= 2*M_PIf32;
                while (d_phase < -M_PIf32) d_phase += 2*M_PIf32;
            }

            return n;
        }
    private:
        const float d_alpha;
        const float d_beta;
        const float d_max_freq;

        float d_freq = 0.0f;
        float d_phase = 0.0f;
        std::vector<float> phases;
};

#endif
