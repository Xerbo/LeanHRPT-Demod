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

#ifndef IO_RAW_IMPL_H
#define IO_RAW_IMPL_H

#include "reader.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <limits>

template<typename T>
class RawFileReader: public FileReader {
    public:
        RawFileReader(std::string filename) : stream(&file) {
            file.open(filename, std::ios::in | std::ios::binary);
            if (!file.is_open()) {
                throw std::invalid_argument("RawFileReader: could not open file");
            }
        }
        size_t read_samples(std::complex<float> *ptr, size_t n) {
            buffer.reserve(n*2);
            stream.read((char *)buffer.data(), n*2*sizeof(T));

            float a = 1.0f;
            float b = 0.0f;
            if (std::numeric_limits<T>::is_integer) {
                a = 1.0f / (float)std::numeric_limits<T>::max();

                if (!std::numeric_limits<T>::is_signed) {
                    a *= 2.0f;
                    b = -1.0f;
                }
            }

            for (size_t i = 0; i < n; i++) {
                ptr[i] = std::complex<float>((float)buffer[i*2]*a + b, (float)buffer[i*2+1]*a + b);
            }

            neof = !stream.eof();
            return stream.gcount()/2/sizeof(T);
        }
        int sample_rate() {
            return -1;
        }
    private:
        std::vector<T> buffer;
        std::filebuf file;
        std::istream stream;
};

#endif
