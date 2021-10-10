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

#ifndef DSP_DC_BLOCKER_H
#define DSP_DC_BLOCKER_H

#include <complex>

// Remove the DC offset from a signal
// NOTE: This is not a true DC blocker, as it uses an exponential moving average
// https://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average
class FastDCBlocker {
    public:
        FastDCBlocker(float alpha) : d_alpha(alpha) { }

        size_t work(const std::complex<float> *in, std::complex<float> *out, size_t n) {
            for (size_t i = 0; i < n; i++) {
                accumulator = d_alpha*in[i] + (1.0f-d_alpha)*accumulator;
                out[i] = in[i] - accumulator;
            }

            return n;
        }
    private:
        const float d_alpha;
        std::complex<float> accumulator;
};

#endif
