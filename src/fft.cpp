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

#include "fft.h"
#include <cstring>

void FFT::work(const std::complex<float> *in, float *out) {
    std::memcpy(d_in, in, sizeof(std::complex<float>) * bins);
    apply_window(d_in, bins);

    fftwf_execute(plan);

    for (size_t i = 0; i < bins/2; i++) {
        out[i]        = power(d_out[i + bins/2]);
        out[i+bins/2] = power(d_out[i]);
    }
}

// https://en.wikipedia.org/wiki/Window_function#Blackman_window
void FFT::init_window() {
    window.resize(bins);
    window_sum = 0.0f;

    for (size_t i = 0; i < bins; i++) {
        float n = ((float)i / (float)bins) * M_PIf;
        window[i] = 0.42f - 0.5f*std::cos(2 * n) + 0.08f*std::cos(4 * n);
        window_sum += window[i];
    }
}

void FFT::apply_window(fftwf_complex *ptr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        ptr[i][0] *= window[i];
        ptr[i][1] *= window[i];
    }
}
