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

#include <set>
#include "io/reader.h"
#include "dsp/dc_blocker.h"
#include "dsp/carrier_pll.h"
#include "dsp/frequency_translator.h"
#include "dsp/fir_filter.h"
#include "dsp/agc.h"
#include "dsp/clock_recovery.h"
#include "dsp/binary_slicer.h"
#include "dsp/complex_to_char.h"
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
        std::shared_ptr<FileReader> file;

        virtual std::vector<complex> &symbols()=0;
        virtual std::vector<complex> &baseband()=0;
        virtual ~Demodulator() { }

        void stop() {
            for (BlockInterface *block : blocks) block->stop();
        }
        void start() {
            for (BlockInterface *block : blocks) block->start();
        }
        bool is_running() {
            return !file->eof;
        }

    protected:
        std::set<BlockInterface *> blocks;

        template<class A, class B> void connect(A *a, B *b) {
            blocks.insert(a);
            blocks.insert(b);
            a->in_pipe = b->out_pipe;
        }
        template<class A, class B> void connect(A &a, std::shared_ptr<B> &b) { connect(&a, b.get()); }
        template<class A, class B> void connect(std::shared_ptr<A> &a, B &b) { connect(a.get(), &b); }
        template<class A, class B> void connect(A &a, B &b)                  { connect(&a, &b); }
};

class BiphaseDemodulator : public Demodulator {
    public:
        BiphaseDemodulator(float samp_rate,
                           float sym_rate,
                           std::shared_ptr<FileReader> source,
                           std::string output_filename);
        std::vector<complex> &symbols() { return slicer.in; }
        std::vector<complex> &baseband() { return pll.in; }

    private:
        FastDCBlocker dc_blocker;
        CarrierPLL pll;
        FrequencyTranslator translator;
        FIRFilter rrc;
        AGC agc;
#ifdef EXPERIMENTAL
        CostasLoopSSE costas_loop;
#else
        CostasLoop costas_loop;
#endif
        SymbolSync clock_recovery;
        BinarySlicer slicer;
        FileWriter<uint8_t> out;
        
};

template<class SymbolHandler, class Deframer, typename OutputType = uint8_t>
class PSKDemodulator : public Demodulator {
    public:
        PSKDemodulator(float samp_rate,
                       float sym_rate,
                       size_t order,
                       bool suppress_carrier,
                       float costas_bw,
                       std::shared_ptr<FileReader> source,
                       std::string output_filename);
        std::vector<complex> &symbols() { return symbol_handler.in; }
        std::vector<complex> &baseband() { return agc.in; }

    private:
        FastDCBlocker dc_blocker;
        FIRFilter rrc;
        AGC agc;
#ifdef EXPERIMENTAL
        CostasLoopSSE costas_loop;
#else
        CostasLoop costas_loop;
#endif
        SymbolSync clock_recovery;
        SymbolHandler symbol_handler;
        Deframer deframer;
        FileWriter<OutputType> out;
};

#endif
