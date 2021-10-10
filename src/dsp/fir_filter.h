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

#ifndef DSP_FIR_FILTER_H
#define DSP_FIR_FILTER_H

#include "fir_kernel.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <complex>

class FIRFilter {
    public:
        FIRFilter(const std::vector<float> &taps) : kernel(taps) {
            ring_buffer.resize(taps.size());
        }

        size_t work(const std::complex<float> *in, std::complex<float> *out, size_t n) {
            ring_buffer.insert(ring_buffer.end(), in, &in[n]);
            kernel.filter(ring_buffer.data(), out, n);
            ring_buffer.erase(ring_buffer.begin(), ring_buffer.end() - kernel.ntaps());

            return n;
        }
    private:
        std::vector<std::complex<float>> ring_buffer;
        FIRKernel kernel;
};

#endif
