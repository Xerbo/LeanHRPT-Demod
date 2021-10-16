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

#include "dsp.h"
#include "util/fir_taps.h"

#include <iostream>

PMDemodulator::PMDemodulator(float SAMP_RATE, std::shared_ptr<FileReader> source, std::string ofname)
    : file(std::move(source)),
      pll(loop(M_PIf/150.0f)),
      ft(2.0f*M_PIf * -665.4e3/SAMP_RATE),
      rrc(make_rrc(1.0, SAMP_RATE, 665.4e3, 0.6, 51)),
      costas(2, loop(0.005f)),
      clock(SAMP_RATE/665.4e3, loop(0.01f)),
      out(ofname) {

    agc.in_pipe = file->out_pipe;
    pll.in_pipe = agc.out_pipe;
    ft.in_pipe = pll.out_pipe;
    rrc.in_pipe = ft.out_pipe;
    costas.in_pipe = rrc.out_pipe;
    clock.in_pipe = costas.out_pipe;
    slicer.in_pipe = clock.out_pipe;
    out.in_pipe = slicer.out_pipe;

    file->start();
    agc.start();
    pll.start();
    ft.start();
    rrc.start();
    costas.start();
    clock.start();
    slicer.start();
    out.start();
}

QPSKDemodulator::QPSKDemodulator(float SAMP_RATE, std::shared_ptr<FileReader> source, std::string ofname)
    : file(std::move(source)),
      rrc(make_rrc(1.0, SAMP_RATE, 2.3333e6, 0.6, 51)),
      costas(4, loop(0.005f)),
      clock(SAMP_RATE/2.3333e6, loop(0.01f)),
      out(ofname) {

    agc.in_pipe = file->out_pipe;
    rrc.in_pipe = agc.out_pipe;
    costas.in_pipe = rrc.out_pipe;
    clock.in_pipe = costas.out_pipe;
    out.in_pipe = clock.out_pipe;

    file->start();
    agc.start();
    rrc.start();
    costas.start();
    clock.start();
    out.start();
}
