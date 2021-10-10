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

#include "fir_intrepolator.h"
#include "interpolator_taps.h"

FIRInterpolator::FIRInterpolator() {
    filters.resize(NSTEPS + 1);

    for (size_t i = 0; i < NSTEPS + 1; i++) {
        std::vector<float> t(&taps[i][0], &taps[i][NTAPS]);
        filters[i] = new FIRKernel(t);
    }
}

FIRInterpolator::~FIRInterpolator() {
    for (FIRKernel *filter : filters) {
        delete filter;
    }
}

std::complex<float> FIRInterpolator::interpolate(const std::complex<float> *input, float mu) {
    int imu = (int)rint(mu * NSTEPS);

    if (imu < 0 || imu > (int)NSTEPS) {
        throw std::runtime_error("FIRInterpolator: imu is out of bounds" );
    }

    return filters[imu]->filter(input);
}
