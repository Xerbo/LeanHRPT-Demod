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

struct complex128 {
    __m128 real;
    __m128 imag;
};

complex128 deinterleave_complex(const std::complex<float> *input);
float sum(__m128 x);
__m128 _mm_abs_ps(__m128 __a);
__m128 atan2_sse(__m128 y, __m128 x);

#endif
