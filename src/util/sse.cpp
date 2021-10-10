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

#include "sse.h"

complex128 deinterleave_complex(const std::complex<float> *input) {
    float *data = (float *)input;
    const __m128 packed0 = _mm_loadu_ps(data);
    const __m128 packed1 = _mm_loadu_ps(data + 4);

    return {
        _mm_shuffle_ps(packed0, packed1, _MM_SHUFFLE(2, 0, 2, 0)),
        _mm_shuffle_ps(packed0, packed1, _MM_SHUFFLE(3, 1, 3, 1))
    };
}

float sum(__m128 x) {
    float values[4];
    _mm_storeu_ps(values, x);
    return values[0] + values[1] + values[2] + values[3];
}

__m128 _mm_abs_ps(__m128 __a) {
    const __m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff));
    return _mm_and_ps(__a, mask);
}

__m128 atan2_sse(__m128 y, __m128 x) {
    const __m128 abs_x = _mm_abs_ps(x);
    const __m128 abs_y = _mm_abs_ps(y);
    const __m128 max = _mm_max_ps(abs_y, abs_x);
    const __m128 min = _mm_min_ps(abs_y, abs_x);
    const __m128 ratio = _mm_div_ps(min, max);

    // Polynomial approximation of atan(ratio) for [0,1]
    const __m128 ratio3 = _mm_mul_ps(_mm_mul_ps(ratio, ratio), ratio);
    __m128 r = _mm_add_ps(ratio, _mm_mul_ps(ratio3, _mm_set_ps1(-0.226f)));

    // Map to a full circle
    const __m128 a_mask = _mm_cmpgt_ps(abs_y, abs_x);
    const __m128 b_mask = _mm_cmplt_ps(x, _mm_set1_ps(0.0f));
    const __m128 c_mask = _mm_cmplt_ps(y, _mm_set1_ps(0.0f));
    r = _mm_or_ps(_mm_and_ps(a_mask, _mm_sub_ps(_mm_set1_ps(1.57079637f), r)), _mm_andnot_ps(a_mask, r));
    r = _mm_or_ps(_mm_and_ps(b_mask, _mm_sub_ps(_mm_set1_ps(3.14159274f), r)), _mm_andnot_ps(b_mask, r));
    r = _mm_or_ps(_mm_and_ps(c_mask, _mm_mul_ps(_mm_set1_ps(-1.0f), r)), _mm_andnot_ps(c_mask, r));

    return r;
}