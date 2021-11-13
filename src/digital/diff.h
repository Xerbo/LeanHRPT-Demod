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

#ifndef LEANHRPT_VITERBI_DIFF_H
#define LEANHRPT_VITERBI_DIFF_H

#include <cstddef>
#include <cstdint>

// Fengyun differential decoder
class FengyunDiff {
    public:
        void work(uint8_t *in_a, uint8_t *in_b, size_t len, uint8_t *out);
    private:
        bool Xin_1 = 0;
        bool Yin_1 = 0;
};

#endif
