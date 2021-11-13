/*
 * LeanHRPT Viterbi
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

#include "diff.h"

#include <bitset>

void FengyunDiff::work(uint8_t *in_a, uint8_t *in_b, size_t len, uint8_t *out) {
    uint8_t diffOut, diffBuffer = 0;
    size_t o = 0;

    for (size_t i = 0; i < len; i++) {
        for(ssize_t j = 7; j >= 0; j--) {
            bool Xin = std::bitset<8>(in_a[i]).test(j);
            bool Yin = std::bitset<8>(in_b[i]).test(j);

            if (Xin_1 == Yin_1) {
                bool Xout = (Xin_1 ^ Xin);
                bool Yout = (Yin_1 ^ Yin);
                diffOut = Xout << 1 | Yout;
            } else {
                bool Xout = (Yin_1 ^ Yin);
                bool Yout = (Xin_1 ^ Xin);
                diffOut = Xout << 1 | Yout;
            }

            Xin_1 = Xin;
            Yin_1 = Yin;

            diffBuffer = diffBuffer << 2 | diffOut;
            if(j == 3 || j == 7) out[o++] = diffBuffer;
        }
    }
}
