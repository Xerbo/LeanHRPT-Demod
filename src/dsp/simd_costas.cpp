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

#include "simd_costas.h"

#include "util/math.h"
#include <immintrin.h>

namespace sse {
    typedef struct {
        __m128 real;
        __m128 imag;
    } complex;

    // Calculate phase error of a BPSK symbol
    __m128 phase_detector_bpsk(complex x) {
        return _mm_mul_ps(x.real, x.imag);
    }

    // Calculate phase error of a QPSK symbol
    __m128 phase_detector_qpsk(complex x) {
        const __m128 realslice_mask = _mm_cmpgt_ps(x.real, _mm_set1_ps(0.0f));
        const __m128 realslice = _mm_or_ps(_mm_and_ps(realslice_mask, _mm_set1_ps(1.0f)), _mm_andnot_ps(realslice_mask, _mm_set1_ps(-1.0f)));
        const __m128 imagslice_mask = _mm_cmpgt_ps(x.imag, _mm_set1_ps(0.0f));
        const __m128 imagslice = _mm_or_ps(_mm_and_ps(imagslice_mask, _mm_set1_ps(1.0f)), _mm_andnot_ps(imagslice_mask, _mm_set1_ps(-1.0f)));

        x.real = _mm_mul_ps(x.real, imagslice);
        x.imag = _mm_mul_ps(x.imag, realslice);

        return _mm_sub_ps(x.real, x.imag);
    }

    // Multiply two complex numbers
    complex complex_multiply(complex a, complex b) {
        return {
            _mm_sub_ps(_mm_mul_ps(a.real, b.real), _mm_mul_ps(a.imag, b.imag)),
            _mm_add_ps(_mm_mul_ps(a.real, b.imag), _mm_mul_ps(a.imag, b.real))
        };
    }

    // AAAA, BBBB -> AB, AB, AB, AB
    complex deinterleave_complex(const std::complex<float> *in) {
        float *data = (float *)in;
        const __m128 packed0 = _mm_loadu_ps(data);
        const __m128 packed1 = _mm_loadu_ps(data + 4);

        return {
            _mm_shuffle_ps(packed0, packed1, _MM_SHUFFLE(2, 0, 2, 0)),
            _mm_shuffle_ps(packed0, packed1, _MM_SHUFFLE(3, 1, 3, 1))
        };
    }

    // AB, AB, AB, AB -> AAAA, BBBB
    void interleave_complex(std::complex<float> *out, complex x) {
        __m128 unpacked0 = _mm_unpacklo_ps(x.real, x.imag);
        __m128 unpacked1 = _mm_unpackhi_ps(x.real, x.imag);

        float *data = (float *)out;
        _mm_storeu_ps(data, unpacked0);
        _mm_storeu_ps(data + 4, unpacked1);
    }

    // Sine and cosine
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

    // Create a quadrate oscillator with phase `x`
    complex create_osc(__m128 x){
        return {
            sse::sin(x),
            _mm_mul_ps(sse::cos(x), _mm_set1_ps(-1.0f))
        };
    }

    // Sum all 4 values in a __m128
    float sum(__m128 x) {
        float y[4];
        _mm_storeu_ps(y, x);
        return y[0] + y[1] + y[2] + y[3];
    }

    float avg(__m128 x) {
        return sum(x) / 4.0f;
    }
}

size_t CostasLoopSSE::work(const std::complex<float> *in, std::complex<float> *out, size_t n) {
    for (size_t i = 0; i < n/4; i++) {
        sse::complex data = sse::deinterleave_complex(&in[i*4]);

        // Calculate the (estimated) phase for each sample
        __m128 phase = _mm_set1_ps(d_phase);
        __m128 offset = _mm_mul_ps(_mm_set1_ps(d_freq), _mm_set_ps(0.0f, 0.25f, 0.5f, 0.75f));
        phase = _mm_add_ps(phase, offset);
    
        // Mix with the oscillator
        data = sse::complex_multiply(data, sse::create_osc(phase));

        sse::interleave_complex(&out[i*4], data);

        // Calculate phase error
        float error;
        switch (d_order) { 
            case 2: error = sse::avg(sse::phase_detector_bpsk(data)); break;
            case 4: error = sse::avg(sse::phase_detector_qpsk(data)); break;
            default: throw std::runtime_error("Invalid or unsupported PSK order");
        }

        // Adjust frequency based on the phase error (clamping if necessary)
        d_freq = d_freq + d_beta * error;
        d_phase = d_phase + d_freq + d_alpha * error;

        // Wrap phase (faster than std::remainder)
        if (d_phase >  M_PIf) d_phase -= 2*M_PIf;
        if (d_phase < -M_PIf) d_phase += 2*M_PIf;
    }

    return n;
}
