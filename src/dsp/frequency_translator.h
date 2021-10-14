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

#ifndef DSP_FREQUENCY_TRANSLATOR_H
#define DSP_FREQUENCY_TRANSLATOR_H

#include "block.h"
#include "util/math.h"
#include <complex>
#include <cmath>

// Translates the quadrate/imaginary branch into the complex domain
class FrequencyTranslator : public Block<complex, complex> {
    public:
        // Frequency is in radians per sample
        FrequencyTranslator(float shift) : d_shift(shift) { }

        size_t work(const std::complex<float> *in, std::complex<float> *out, size_t n) {
            for (size_t i = 0; i < n; i++) {
                out[i] = in[i].imag() * std::complex<float>(fast_cos(d_phase), -fast_sin(d_phase));
                d_phase = std::remainder(d_phase + d_shift, M_PIf * 2.0f);
            }

            return n;
        }
    private:
        const float d_shift;
        float d_phase = 0.0f;
};

#endif
