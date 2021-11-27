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

#include "math.h"

#include <algorithm>
#include <cmath>

float clamp(float v, float lo, float hi) {
    return std::max(lo, std::min(hi, v));
}

float clamp(float v, float hi) {
    return clamp(v, -hi, hi);
}

std::pair<float, float> loop(float bandwidth, float damping) {
    float denom = (1.0f + 2.0f * damping * bandwidth + bandwidth * bandwidth);
    float alpha = (4.0f * damping * bandwidth) / denom;
    float beta  = (4.0f * bandwidth * bandwidth) / denom;

    return {alpha, beta};
}

float fast_sin(float x) {
    float abs_x = std::abs(x);

    float numerator   = 16.0f*abs_x * (M_PIf32 - abs_x);
    float denominator = 5.0f*M_PIf32*M_PIf32 - 4.0f*abs_x*(M_PIf32 - abs_x);

    return std::copysign(numerator/denominator, x);
}

float fast_cos(float x) {
    x += M_PIf32/2.0f;
    if (x > M_PIf32) x -= M_PIf32*2.0f;

    return fast_sin(x);
}
