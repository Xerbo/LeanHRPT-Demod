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

BiphaseDemodulator::BiphaseDemodulator(float samp_rate,
                                       float sym_rate,
                                       std::shared_ptr<FileReader> source,
                                       std::string output_filename)
    : dc_blocker(0.001f),
      pll(loop(0.005f), M_TAUf32 * 150e3f/samp_rate),
      translator(M_TAUf32 * -sym_rate/samp_rate),
      rrc(make_rrc(1.0, samp_rate, sym_rate, 0.6, 51)),
      agc(0.001f, 0.707f),
      costas_loop(2, loop(0.005f), 0.0f),
      clock_recovery(2, samp_rate/sym_rate, loop(0.01f)),
      out(output_filename) {

    file = std::move(source);
    connect(dc_blocker, file);
    connect(pll, dc_blocker);
    connect(translator, pll);
    connect(rrc, translator);
    connect(agc, rrc);
    connect(costas_loop, agc);
    connect(clock_recovery, costas_loop);
    connect(slicer, clock_recovery);    
    connect(out, slicer);
    start();
}

template<class SymbolHandler, class Deframer>
PSKDemodulator<SymbolHandler, Deframer>::PSKDemodulator(float samp_rate,
                                                        float sym_rate,
                                                        size_t order,
                                                        bool suppress_carrier,
                                                        std::shared_ptr<FileReader> source,
                                                        std::string output_filename)
    : dc_blocker(0.001f),
      rrc(make_rrc(1.0, samp_rate, sym_rate, 0.6, 51)),
      costas_loop(order, loop(0.005f), M_TAUf32 * 150e3f/samp_rate, suppress_carrier),
      clock_recovery(order, samp_rate/sym_rate, loop(0.01f)),
      deframer(QFileInfo(QString::fromStdString(output_filename)).suffix().toStdString()),
      out(output_filename) {

    file = std::move(source);
    connect(dc_blocker, file);
    connect(agc, dc_blocker);
    connect(rrc, agc);
    connect(costas_loop, rrc);
    connect(clock_recovery, costas_loop);
    connect(symbol_handler, clock_recovery);
    connect(deframer, symbol_handler);
    connect(out, deframer);
    start();
}

template class PSKDemodulator<MetopViterbi,   VCDUExtractor>;
template class PSKDemodulator<FengyunViterbi, VCDUExtractor>;
template class PSKDemodulator<Fengyun3CViterbi, VCDUExtractor>;
template class PSKDemodulator<BinarySlicer,   Passthrough<uint8_t>>;
