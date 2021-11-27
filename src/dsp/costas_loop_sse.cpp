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

#include "costas_loop_sse.h"

#include "util/math.h"
#include "util/sse.h"
#include <immintrin.h>

namespace sse {
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

    // Create a quadrate oscillator with phase x+pi/2
    complex create_osc(__m128 x){
        return {
            cos(x),
            _mm_mul_ps(sin(x), _mm_set1_ps(-1.0f))
        };
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
        if (d_phase >  M_PIf32) d_phase -= 2*M_PIf32;
        if (d_phase < -M_PIf32) d_phase += 2*M_PIf32;
    }

    return n;
}
