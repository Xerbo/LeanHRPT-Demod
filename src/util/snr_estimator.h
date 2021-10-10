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

#ifndef UTIL_SNR_ESTIMATOR_H
#define UTIL_SNR_ESTIMATOR_H

#include <cmath>
#include <complex>

class SNREstimator {
    public:
        SNREstimator(double alpha = 0.001)
            : d_alpha(alpha),
              d_beta(1.0 - alpha) { }

        void push(std::complex<float> *input, size_t n) {
            for (size_t i = 0; i < n; i++) {
                double y1 = abs(input[i]) * abs(input[i]);
                d_y1 = d_alpha * y1 + d_beta * d_y1;

                double y2 = abs(input[i]) * abs(input[i]) * abs(input[i]) * abs(input[i]);
                d_y2 = d_alpha * y2 + d_beta * d_y2;
            }
        }
        double get_snr() {
            double y1_2 = d_y1 * d_y1;
            d_signal = sqrt(2 * y1_2 - d_y2);
            d_noise = d_y1 - sqrt(2 * y1_2 - d_y2);
            return 10.0 * log10(d_signal / d_noise);
        }

        double get_snr(std::complex<float> *input, size_t n) {
            push(input, n);
            return get_snr();
        }
    private:
        double d_y1 = 0.0;
        double d_y2 = 0.0;
        double d_signal = 0.0;
        double d_noise = 0.0;
        const double d_alpha;
        const double d_beta;
};

#endif
