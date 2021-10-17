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

class FileReader : public Block<Empty, complex> {
    public:
        virtual size_t read_samples(std::complex<float> *ptr, size_t n)=0;
        virtual int sample_rate()=0;
        static std::shared_ptr<FileReader> choose_type(std::string type, std::string filename);
        bool neof = true;

    private:
        size_t work(complex *out, size_t n) {
            return read_samples(out, n);
        }
};

#endif
