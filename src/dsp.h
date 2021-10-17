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
#include "util/pipe.hh"
#include "io/writer.h"

#include <fstream>
#ifdef _WIN32
#include "mingw.thread.h"
#else
#include <thread>
#endif

class Demodulator {
    public:
        bool running = true;
        virtual std::vector<complex> &symbols()=0;
        virtual bool is_running()=0;
        virtual void stop()=0;
};

class PMDemodulator : public Demodulator {
    public:
        PMDemodulator(float SAMP_RATE, std::shared_ptr<FileReader> source, std::string ofname);
        std::vector<complex> &symbols() {
            return slicer.in;
        }
        bool is_running() {
            return file->neof;
        }
        void stop();
    private:
        std::shared_ptr<FileReader> file;
        AGC agc;
        CarrierPLL pll;
        FrequencyTranslator ft;
        FIRFilter rrc;
        CostasLoop costas;
        SymbolSync clock;
        BinarySlicer slicer;
        FileWriter<uint8_t> out;
};

class QPSKDemodulator : public Demodulator {
    public:
        QPSKDemodulator(float SAMP_RATE, std::shared_ptr<FileReader> source, std::string ofname);
        std::vector<complex> &symbols() {
            return out.in;
        }
        bool is_running() {
            return file->neof;
        }
        void stop();
    private:
        std::shared_ptr<FileReader> file;
        AGC agc;
        FIRFilter rrc;
        CostasLoop costas;
        SymbolSync clock;
        FileWriter<complex> out;
};

#endif
