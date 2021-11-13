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

#ifndef DSP_CUBIC_INTREPOLATOR_H
#define DSP_CUBIC_INTREPOLATOR_H

#include <complex>

// Interpolator based off a cubic Bézier curve, should be a drop in replacment for FIRInterpolator
class CubicInterpolator {
    public:
        std::complex<float> interpolate(const std::complex<float> *input, float mu) {
            mu = mu*0.5f + 0.5f;

            std::complex<float> a = lerp(input[2], input[3], mu);
            std::complex<float> b = lerp(input[3], input[4], mu);
            std::complex<float> c = lerp(input[4], input[5], mu);

            std::complex<float> a1 = lerp(a, b, mu);
            std::complex<float> b1 = lerp(b, c, mu);

            return lerp(a1, b1, mu);
        }

        size_t ntaps() {
            return 8;
        }
    private:
        inline std::complex<float> lerp(std::complex<float> a, std::complex<float> b, float x) {
           return a*(1.0f-x) + b*x;
        }
};

#endif
