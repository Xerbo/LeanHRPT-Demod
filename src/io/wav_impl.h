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

#ifndef IO_WAV_IMPL_H
#define IO_WAV_IMPL_H

#include "reader.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <sndfile.hh>

class WAVFileReader: public FileReader {
    public:
        WAVFileReader(std::string filename) : file(filename, SFM_READ) {
            if (file.error() != 0) {
                throw std::invalid_argument("WAVFileReader: " + std::string(file.strError()));
            }
            if (file.channels() != 2) {
                throw std::invalid_argument("WAVFileReader: file does not have 2 channels");
            }
            if (file.samplerate() < 1e6) {
                throw std::invalid_argument("WAVFileReader: sample rate not high enough");
            }
        }
        bool read_samples(std::complex<float> *ptr, size_t n) {
            buffer.reserve(n*2);
            size_t read = file.readf(buffer.data(), n);

            for (size_t i = 0; i < read; i++) {
                ptr[i] = std::complex<float>(buffer[i*2], buffer[i*2 + 1]);
            }

            return read == n;
        }
        int sample_rate() {
            return file.samplerate();
        }
    private:
        std::vector<float> buffer;
        SndfileHandle file;
};

#endif
