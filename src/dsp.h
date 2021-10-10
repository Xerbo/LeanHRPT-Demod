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

#ifndef DSP_H
#define DSP_H

#include "io/reader.h"
#include "dsp/carrier_pll.h"
#include "dsp/frequency_translator.h"
#include "dsp/fir_filter.h"
#include "dsp/agc.h"
#include "dsp/costas_loop.h"
#include "dsp/clock_recovery.h"
#include "dsp/binary_slicer.h"
#include "util/pipe.h"

#include <fstream>
#include <thread>

class PMDemodulator {
    public:
        PMDemodulator(float SAMP_RATE, std::shared_ptr<FileReader> source);
        std::vector<std::complex<float>> symbols;
        bool running = true;
    private:
        std::shared_ptr<FileReader> file;
        AGC agc;
        CarrierPLL pll;
        FrequencyTranslator ft;
        FIRFilter rrc;
        CostasLoop costas;
        ClockRecovery clock;
        BinarySlicer slicer;
        std::ofstream of;

        Pipe<std::complex<float>> file_pipe;
        Pipe<std::complex<float>> pll_pipe;
        Pipe<std::complex<float>> ft_pipe;
        Pipe<std::complex<float>> rrc_pipe;
        Pipe<std::complex<float>> costas_pipe;

        std::thread *a;
        std::thread *b;
        std::thread *c;
        std::thread *d;
        std::thread *e;
        std::thread *f;
};

#endif
