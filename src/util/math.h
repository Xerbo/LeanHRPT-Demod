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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PIf32
#define M_PIf32 3.141592653589793238462643383279502884f
#endif
#ifndef M_TAU
#define M_TAU 6.28318530717958647692
#endif
#ifndef M_TAUf32
#define M_TAUf32 6.283185307179586476925286766559005768f
#endif

#ifndef UTIL_MATH_H
#define UTIL_MATH_H

#include <utility>

float clamp(float v, float lo, float hi);
float clamp(float v, float hi);

// https://www.trondeau.com/blog/2011/8/13/control-loop-gain-values.html
std::pair<float, float> loop(float bandwidth, float damping = 0.707f);

// https://en.wikipedia.org/wiki/Bhaskara_I's_sine_approximation_formula
// Only works within the bounds of [-M_PI, M_PI]
float fast_sin(float x);
float fast_cos(float x);

#endif
