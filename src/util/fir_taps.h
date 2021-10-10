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

#ifndef UTIL_FIR_TAPS_H
#define UTIL_FIR_TAPS_H

#include "math.h"
#include <vector>
#include <cmath>

// From https://github.com/gnuradio/gnuradio/blob/master/gr-filter/lib/firdes.cc#L641-L684
static std::vector<float> make_rrc(double gain, double sampling_freq, double symbol_rate, double alpha, int ntaps) {
    ntaps |= 1; // ensure that ntaps is odd

    double spb = sampling_freq / symbol_rate; // samples per bit/symbol
    std::vector<float> taps(ntaps);
    double scale = 0;
    for (int i = 0; i < ntaps; i++) {
        double x1, x2, x3, num, den;
        double xindx = i - ntaps / 2;
        x1 = M_PI * xindx / spb;
        x2 = 4 * alpha * xindx / spb;
        x3 = x2 * x2 - 1;

        if (fabs(x3) >= 0.000001) { // Avoid Rounding errors...
            if (i != ntaps / 2)
                num = cos((1 + alpha) * x1) +
                      sin((1 - alpha) * x1) / (4 * alpha * xindx / spb);
            else
                num = cos((1 + alpha) * x1) + (1 - alpha) * M_PI / (4 * alpha);
            den = x3 * M_PI;
        } else {
            if (alpha == 1) {
                taps[i] = -1;
                scale += taps[i];
                continue;
            }
            x3 = (1 - alpha) * x1;
            x2 = (1 + alpha) * x1;
            num = (sin(x2) * (1 + alpha) * M_PI -
                   cos(x3) * ((1 - alpha) * M_PI * spb) / (4 * alpha * xindx) +
                   sin(x3) * spb * spb / (4 * alpha * xindx * xindx));
            den = -32 * M_PI * alpha * alpha * xindx / spb;
        }
        taps[i] = 4 * alpha * num / den;
        scale += taps[i];
    }

    for (int i = 0; i < ntaps; i++)
        taps[i] = taps[i] * gain / scale;

    return taps;
}

#endif
