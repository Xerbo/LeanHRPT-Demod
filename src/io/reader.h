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

#ifndef IO_READER_H
#define IO_READER_H

#include <complex>
#include <memory>
#include "dsp/block.h"
#include <SoapySDR/Types.hpp>

class FileReader : public Block<Empty, complex> {
    public:
        virtual size_t read_samples(std::complex<float> *ptr, size_t n)=0;
        virtual double rate() {
            return -1;
        };

        // (SDR only) gain control
        virtual void set_gain(const std::string &name, double gain) {
            (void)name;
            (void)gain;
        };
        virtual SoapySDR::Range gain_range(const std::string &name) {
            (void)name;
            return SoapySDR::Range();
        }
        virtual std::vector<std::string> get_gains() {
            return {};
        }

        // (SDR only) bias tee control
        virtual void set_biastee(double enabled) {
            (void)enabled;
        };
        virtual bool has_biastee() {
            return false;
        };

        // (SDR only) antenna control
        virtual std::vector<std::string> antennas() {
            return {};
        }
        virtual std::string antenna() {
            return "";
        }
        virtual void set_antenna(const std::string &antenna) {
            (void)antenna;
        }

        static std::shared_ptr<FileReader> choose_type(std::string type, std::string filename);
        bool eof = false;

    private:
        size_t work(complex *out, size_t n) {
            return read_samples(out, n);
        }
};

#endif
