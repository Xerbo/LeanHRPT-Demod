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
#define M_PIf (float)M_PI

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
