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

#ifndef FFT_H
#define FFT_H

#include "util/math.h"

#include <complex>
#include <vector>
#include <cmath>
#include <fftw3.h>

class FFT {
    public:
        FFT(size_t n) : bins(n) {
            init_window();
            d_in = (fftwf_complex *)fftwf_malloc(bins * sizeof(fftwf_complex));
            d_out = (fftwf_complex *)fftwf_malloc(bins * sizeof(fftwf_complex));
            plan = fftwf_plan_dft_1d(bins, d_in, d_out, FFTW_FORWARD, FFTW_MEASURE);
        }
        ~FFT() {
            fftwf_destroy_plan(plan);
            fftwf_free(d_in);
            fftwf_free(d_out);
        }

        void work(const std::complex<float> *in, float *out);
    private:
        fftwf_complex *d_in;
        fftwf_complex *d_out;
        fftwf_plan plan;
        size_t bins;

        std::vector<float> window;
        float window_sum;
        void init_window();
        void apply_window(fftwf_complex *ptr, size_t n);

        // Complex FFT to dB
        inline float power(fftwf_complex sample) {
            float power = std::sqrt(std::pow(sample[0], 2) + std::pow(sample[1], 2)) * 2.0f/window_sum;
            float db = std::log10(power) * 20.0f;
            return db;
        }
};

#endif
