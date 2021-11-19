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

#ifndef DSP_SIMD_COSTAS_H
#define DSP_SIMD_COSTAS_H

#include "block.h"

// Recovers carrier phase and outputs the input signal mixed with the carrier
// This is the vectorized version, faster but *very* slightly less accurate
class CostasLoopSSE : public Block<complex, complex> {
    public:
        CostasLoopSSE(size_t order, std::pair<float, float> loop)
            : d_order(order),
              d_alpha(loop.first),
              d_beta(loop.second) { }

        size_t work(const std::complex<float> *in, std::complex<float> *out, size_t n);
    private:
        const size_t d_order;
        const float d_alpha;
        const float d_beta;

        float d_freq = 0.0f;
        float d_phase = 0.0f;
};

#endif
