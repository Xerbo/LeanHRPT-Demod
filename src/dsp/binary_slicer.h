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

#ifndef DSP_BINARY_SLICER_H
#define DSP_BINARY_SLICER_H

#include "block.h"
#include <complex>

class BinarySlicer : public Block<complex, uint8_t> {
    public:
        size_t work(const std::complex<float> *in, uint8_t *out, size_t n) {
            size_t j = 0;

            for (size_t i = 0; i < n; i++) {
                buffer = buffer << 1 | (in[i].real() > 0.0f);
                bit++;

                if (bit == 8) {
                    out[j++] = buffer;
                    bit = 0;
                }
            }

            return j;
        }
    private:
        uint8_t buffer = 0x00;
        size_t bit = 0;
};

#endif
