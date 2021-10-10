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

#ifndef DSP_FIR_IMPL_H
#define DSP_FIR_IMPL_H

#include <complex>
#include <vector>
#include <algorithm>

class FIRKernel {
    public:
        FIRKernel(const std::vector<float> &taps) {
            set_taps(taps);
        }
        void set_taps(const std::vector<float> &taps) {
            d_taps = taps;
            std::reverse(d_taps.begin(), d_taps.end());
        }
        size_t ntaps() {
            return d_taps.size();
        }

        std::complex<float> filter(const std::complex<float> *in);
        void filter(const std::complex<float> *in, std::complex<float> *out, size_t n);
    private:
        std::vector<float> d_taps;
};

#endif
