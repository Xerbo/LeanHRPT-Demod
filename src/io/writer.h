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

#ifndef IO_WRITER_H
#define IO_WRITER_H

#include <fstream>
#include "dsp/block.h"

template<typename T>
class FileWriter : public Block<T, Empty> {
    public:
        FileWriter(std::string filename) : stream(&file) {
            file.open(filename, std::ios::out | std::ios::binary);
            if (!file.is_open()) {
                throw std::runtime_error("FileWriter: Could not open file");
            }
        }
        void work(const T *in, size_t n) {
            stream.write((char *)in, n*sizeof(T));
        }
    private:
        std::filebuf file;
        std::ostream stream;
};

#endif
