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
#include "dsp/dc_blocker.h"
#include "dsp/carrier_pll.h"
#include "dsp/frequency_translator.h"
#include "dsp/fir_filter.h"
#include "dsp/agc.h"
#include "dsp/clock_recovery.h"
#include "dsp/binary_slicer.h"
#include "util/pipe.hh"
#include "io/writer.h"
#include "digital/blocks.h"

#ifdef EXPERIMENTAL
#include "dsp/costas_loop_sse.h"
#else
#include "dsp/costas_loop.h"
#endif

class Demodulator {
    public:
        bool running = true;
        std::shared_ptr<FileReader> file;
        virtual std::vector<complex> &symbols()=0;
        virtual std::vector<complex> &freq()=0;
        virtual bool is_running()=0;
        virtual void stop()=0;
        virtual ~Demodulator() { }
};

class PMDemodulator : public Demodulator {
    public:
        PMDemodulator(float SAMP_RATE, std::shared_ptr<FileReader> source, std::string ofname);
        std::vector<complex> &symbols() {
            return slicer.in;
        }
        std::vector<complex> &freq() {
            return pll.in;
        }
        bool is_running() {
            return file->neof;
        }
        void stop();
    private:
        FastDCBlocker dc;
        CarrierPLL pll;
        FrequencyTranslator ft;
        FIRFilter rrc;
        AGC agc;
#ifdef EXPERIMENTAL
        CostasLoopSSE costas;
#else
        CostasLoop costas;
#endif
        SymbolSync clock;
        BinarySlicer slicer;
        FileWriter<uint8_t> out;
        std::vector<BlockInterface *> blocks;
};

class MetopDemodulator : public Demodulator {
    public:
        MetopDemodulator(float SAMP_RATE, std::shared_ptr<FileReader> source, std::string ofname);
        std::vector<complex> &symbols() {
            return viterbi.in;
        }
        std::vector<complex> &freq() {
            return agc.in;
        }
        bool is_running() {
            return file->neof;
        }
        void stop();
    private:
        FastDCBlocker dc;
        AGC agc;
        FIRFilter rrc;
#ifdef EXPERIMENTAL
        CostasLoopSSE costas;
#else
        CostasLoop costas;
#endif
        SymbolSync clock;
        MetopViterbi viterbi;
        VCDUExtractor deframer;
        FileWriter<uint8_t> out;
        std::vector<BlockInterface *> blocks;
};

class FengyunDemodulator : public Demodulator {
    public:
        FengyunDemodulator(float SAMP_RATE, std::shared_ptr<FileReader> source, std::string ofname);
        std::vector<complex> &symbols() {
            return viterbi.in;
        }
        std::vector<complex> &freq() {
            return agc.in;
        }
        bool is_running() {
            return file->neof;
        }
        void stop();
    private:
        FastDCBlocker dc;
        AGC agc;
        FIRFilter rrc;
#ifdef EXPERIMENTAL
        CostasLoopSSE costas;
#else
        CostasLoop costas;
#endif
        SymbolSync clock;
        FengyunViterbi viterbi;
        VCDUExtractor deframer;
        FileWriter<uint8_t> out;
        std::vector<BlockInterface *> blocks;
};

class GACDemodulator : public Demodulator {
    public:
        GACDemodulator(float SAMP_RATE, std::shared_ptr<FileReader> source, std::string ofname);
        std::vector<complex> &symbols() {
            return slicer.in;
        }
        std::vector<complex> &freq() {
            return agc.in;
        }
        bool is_running() {
            return file->neof;
        }
        void stop();
    private:
        FastDCBlocker dc;
        AGC agc;
        FIRFilter rrc;
#ifdef EXPERIMENTAL
        CostasLoopSSE costas;
#else
        CostasLoop costas;
#endif
        SymbolSync clock;
        BinarySlicer slicer;
        FileWriter<uint8_t> out;
        std::vector<BlockInterface *> blocks;
};

#endif
