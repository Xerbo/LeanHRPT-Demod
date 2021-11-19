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

namespace sse {
    // AB, AB, AB, AB -> AAAA, BBBB
    complex deinterleave_complex(const std::complex<float> *in) {
        float *data = (float *)in;
        const __m128 packed0 = _mm_loadu_ps(data);
        const __m128 packed1 = _mm_loadu_ps(data + 4);

        return {
            _mm_shuffle_ps(packed0, packed1, _MM_SHUFFLE(2, 0, 2, 0)),
            _mm_shuffle_ps(packed0, packed1, _MM_SHUFFLE(3, 1, 3, 1))
        };
    }

    // AAAA, BBBB -> AB, AB, AB, AB
    void interleave_complex(std::complex<float> *out, complex x) {
        __m128 unpacked0 = _mm_unpacklo_ps(x.real, x.imag);
        __m128 unpacked1 = _mm_unpackhi_ps(x.real, x.imag);

        float *data = (float *)out;
        _mm_storeu_ps(data, unpacked0);
        _mm_storeu_ps(data + 4, unpacked1);
    }

    complex complex_multiply(complex a, complex b) {
        return {
            _mm_sub_ps(_mm_mul_ps(a.real, b.real), _mm_mul_ps(a.imag, b.imag)),
            _mm_add_ps(_mm_mul_ps(a.real, b.imag), _mm_mul_ps(a.imag, b.real))
        };
    }

    // Sine
    __m128 sin(__m128 x) {
        float y[4];
        _mm_storeu_ps(y, x);

        return _mm_set_ps(
            sinf(y[0]),
            sinf(y[1]),
            sinf(y[2]),
            sinf(y[3])
        );
    }

    // Cosine
    __m128 cos(__m128 x) {
        float y[4];
        _mm_storeu_ps(y, x);

        return _mm_set_ps(
            cosf(y[0]),
            cosf(y[1]),
            cosf(y[2]),
            cosf(y[3])
        );
    }

    // Absolute value
    __m128 abs(__m128 __a) {
        const __m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff));
        return _mm_and_ps(__a, mask);
    }

    __m128 abs(complex x) {
        __m128 real = _mm_mul_ps(x.real, x.real);
        __m128 imag = _mm_mul_ps(x.imag, x.imag);
        return sqrt(_mm_add_ps(real, imag));
    }

    // 2D Arctangent
    __m128 atan2(__m128 y, __m128 x) {
        const __m128 abs_x = abs(x);
        const __m128 abs_y = abs(y);
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

    float sum(__m128 x) {
        float y[4];
        _mm_storeu_ps(y, x);
        return y[0] + y[1] + y[2] + y[3];
    }

    float avg(__m128 x) {
        return sum(x) / 4.0f;
    }

    __m128 sqrt(__m128 x) {
        return _mm_rcp_ps(_mm_rsqrt_ps(x));
    }
}
