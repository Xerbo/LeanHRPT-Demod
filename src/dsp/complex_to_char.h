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

#ifndef COMPLEX_TO_CHAR_H
#define COMPLEX_TO_CHAR_H

#include "block.h"
#include "util/math.h"

class ComplexToChar : public Block<complex, std::complex<char>> {
    public:
        // Frequency is in radians per sample
        ComplexToChar(float scale = 0.8f) : d_scale(scale) { }

        size_t work(const complex *in, std::complex<char> *out, size_t n) {
            for (size_t i = 0; i < n; i++) {
                out[i].real(clamp(in[i].real()/d_scale, 1.0f) * 127.0f);
                out[i].imag(clamp(in[i].imag()/d_scale, 1.0f) * 127.0f);
            }
            return n;
        }
    private:
        const float d_scale;
};

#endif
