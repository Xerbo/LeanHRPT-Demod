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

const size_t BUFFER_SIZE = 8192;
const float SYM_RATE = 665.4e3;

PMDemodulator::PMDemodulator(float SAMP_RATE, std::shared_ptr<FileReader> source, std::string ofname)
    : symbols(BUFFER_SIZE),
      file(std::move(source)),
      pll(loop(M_PIf/150.0f)),
      ft(2.0f*M_PIf * -SYM_RATE/SAMP_RATE),
      rrc(make_rrc(1.0, SAMP_RATE, SYM_RATE, 0.6, 51)),
      costas(2, loop(0.005f)),
      clock(SAMP_RATE/SYM_RATE),
      file_pipe(BUFFER_SIZE*10), 
      pll_pipe(BUFFER_SIZE*10),
      ft_pipe(BUFFER_SIZE*10), 
      rrc_pipe(BUFFER_SIZE*10), 
      costas_pipe(BUFFER_SIZE*10) {

    outfile = new std::filebuf;
    outfile->open(ofname, std::ios::out | std::ios::binary);
    outstream = new std::ostream(outfile);

    a = new std::thread([&] {
        std::vector<std::complex<float>> in(BUFFER_SIZE);
        std::vector<std::complex<float>> out(BUFFER_SIZE);
        while (running) {
            running = file->read_samples(in.data(), BUFFER_SIZE);
            agc.work(in.data(), out.data(), BUFFER_SIZE);
            file_pipe.push(out.data(), BUFFER_SIZE);
        }
    });

    b = new std::thread([&] {
        std::vector<std::complex<float>> in(BUFFER_SIZE);
        std::vector<std::complex<float>> out(BUFFER_SIZE);
        while (running) {
            size_t n = file_pipe.pop(in.data(), BUFFER_SIZE);
            if (n != 0) {
                pll.work(in.data(), out.data(), BUFFER_SIZE);
                pll_pipe.push(out.data(), BUFFER_SIZE);
            }
        }
    });

    c = new std::thread([&] {
        std::vector<std::complex<float>> in(BUFFER_SIZE);
        std::vector<std::complex<float>> out(BUFFER_SIZE);
        while (running) {
            if (pll_pipe.pop(in.data(), BUFFER_SIZE) != 0) {
                ft.work(in.data(), out.data(), BUFFER_SIZE);
                ft_pipe.push(out.data(), BUFFER_SIZE);
            }
        }
    });

    d = new std::thread([&] {
        std::vector<std::complex<float>> in(BUFFER_SIZE);
        std::vector<std::complex<float>> out(BUFFER_SIZE);
        while (running) {
            if (ft_pipe.pop(in.data(), BUFFER_SIZE) != 0) {
                rrc.work(in.data(), out.data(), BUFFER_SIZE);
                rrc_pipe.push(out.data(), BUFFER_SIZE);
            }
        }
    });

    e = new std::thread([&] {
        std::vector<std::complex<float>> in(BUFFER_SIZE);
        std::vector<std::complex<float>> out(BUFFER_SIZE);
        while (running) {
            if (rrc_pipe.pop(in.data(), BUFFER_SIZE) != 0) {
                costas.work(in.data(), out.data(), BUFFER_SIZE);
                costas_pipe.push(out.data(), BUFFER_SIZE);
            }
        }
    });

    f = new std::thread([&] {
        std::vector<std::complex<float>> in(BUFFER_SIZE);
        std::vector<uint8_t> data(BUFFER_SIZE);
        while (running) {
            if (costas_pipe.pop(in.data(), BUFFER_SIZE) != 0) {
                size_t n = clock.work(in.data(), symbols.data(), BUFFER_SIZE);
                n = slicer.work(symbols.data(), data.data(), n);
                outstream->write((char *)data.data(), n);
            }
        }
    });
}

PMDemodulator::~PMDemodulator() {
    outfile->close();
    delete outfile;
    delete outstream;
}
