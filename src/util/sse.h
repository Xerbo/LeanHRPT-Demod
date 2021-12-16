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

#ifndef UTIL_SSE_H
#define UTIL_SSE_H

#include <immintrin.h>
#include <complex>

namespace sse {
    struct complex {
        __m128 real;
        __m128 imag;
    };

    complex deinterleave_complex(const std::complex<float> *in);
    void interleave_complex(std::complex<float> *out, complex x);

    complex complex_multiply(complex a, complex b);
    complex complex_multiply(complex a, __m128 b);
    inline complex complex_multiply(__m128 a, complex b) { return complex_multiply(b, a); }

    float sum(__m128 x);
    float avg(__m128 x);

    __m128 copysign(__m128 mag, __m128 sign);
    __m128 phase_wrap(__m128 x);

    __m128 sin(__m128 x);
    __m128 cos(__m128 x);
    __m128 abs(__m128 x);
    __m128 abs(complex x);
    __m128 atan2(__m128 y, __m128 x);
    inline __m128 atan2(complex x) { return atan2(x.imag, x.real); }
    __m128 sqrt(__m128 x);
    __m128 fast_div(__m128 a, __m128 b);
    __m128 inv(__m128 x);
}

#endif
