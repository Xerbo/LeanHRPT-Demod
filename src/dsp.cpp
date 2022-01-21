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
#include <QFileInfo>

PMDemodulator::PMDemodulator(float SAMP_RATE, std::shared_ptr<FileReader> source, std::string ofname)
    : dc(0.001f),
      pll(loop(0.005f), M_TAUf32 * 150e3f/SAMP_RATE),
      ft(2.0f*M_PIf32 * -665.4e3/SAMP_RATE),
      rrc(make_rrc(1.0, SAMP_RATE, 665.4e3, 0.6, 51)),
      agc(0.001f, 0.707f),
      costas(2, loop(0.005f), 0.0f),
      clock(2, SAMP_RATE/665.4e3, loop(0.01f)),
      out(ofname) {

    file = std::move(source);

    dc.in_pipe = file->out_pipe;
    pll.in_pipe = dc.out_pipe;
    ft.in_pipe = pll.out_pipe;
    rrc.in_pipe = ft.out_pipe;
    agc.in_pipe = rrc.out_pipe;
    costas.in_pipe = agc.out_pipe;
    clock.in_pipe = costas.out_pipe;
    slicer.in_pipe = clock.out_pipe;
    out.in_pipe = slicer.out_pipe;

    blocks = { file.get(), &dc, &pll, &ft, &rrc, &agc, &costas, &clock, &slicer, &out };
    for (BlockInterface *block : blocks) {
        block->start();
    }
}

void PMDemodulator::stop() {
    for (BlockInterface *block : blocks) {
        block->stop();
    }
}

MetopDemodulator::MetopDemodulator(float SAMP_RATE, std::shared_ptr<FileReader> source, std::string ofname)
    : dc(0.001f),
      rrc(make_rrc(1.0, SAMP_RATE, 2.3333e6, 0.6, 51)),
      costas(4, loop(0.005f), M_TAUf32 * 150e3f/SAMP_RATE),
      clock(4, SAMP_RATE/2.3333e6, loop(0.01f)),
      deframer(QFileInfo(QString::fromStdString(ofname)).suffix() == "cadu"),
      out(ofname) {

    file = std::move(source);

    dc.in_pipe = file->out_pipe;
    agc.in_pipe = dc.out_pipe;
    rrc.in_pipe = agc.out_pipe;
    costas.in_pipe = rrc.out_pipe;
    clock.in_pipe = costas.out_pipe;
    viterbi.in_pipe = clock.out_pipe;
    deframer.in_pipe = viterbi.out_pipe;
    out.in_pipe = deframer.out_pipe;

    blocks = { file.get(), &dc, &agc, &rrc, &costas, &clock, &viterbi, &deframer, &out };
    for (BlockInterface *block : blocks) {
        block->start();
    }
}

void MetopDemodulator::stop() {
    for (BlockInterface *block : blocks) {
        block->stop();
    }
}

FengyunDemodulator::FengyunDemodulator(float SAMP_RATE, std::shared_ptr<FileReader> source, std::string ofname)
    : dc(0.001f),
      rrc(make_rrc(1.0, SAMP_RATE, 2.8e6, 0.8, 51)),
      costas(4, loop(0.005f), M_TAUf32 * 150e3f/SAMP_RATE),
      clock(4, SAMP_RATE/2.8e6, loop(0.01f)),
      deframer(QFileInfo(QString::fromStdString(ofname)).suffix() == "cadu"),
      out(ofname) {

    file = std::move(source);

    dc.in_pipe = file->out_pipe;
    agc.in_pipe = dc.out_pipe;
    rrc.in_pipe = agc.out_pipe;
    costas.in_pipe = rrc.out_pipe;
    clock.in_pipe = costas.out_pipe;
    viterbi.in_pipe = clock.out_pipe;
    deframer.in_pipe = viterbi.out_pipe;
    out.in_pipe = deframer.out_pipe;

    blocks = { file.get(), &dc, &agc, &rrc, &costas, &clock, &viterbi, &deframer, &out };
    for (BlockInterface *block : blocks) {
        block->start();
    }
}

void FengyunDemodulator::stop() {
    for (BlockInterface *block : blocks) {
        block->stop();
    }
}
