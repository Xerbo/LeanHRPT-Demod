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

#include "reader.h"
#include "raw_impl.h"
#include "wav_impl.h"

std::shared_ptr<FileReader> FileReader::choose_type(std::string type, std::string filename) {
    if (type == "char") {
        return std::make_shared<RawFileReader<int8_t>>(filename);
    } else if (type == "uchar") {
        return std::make_shared<RawFileReader<uint8_t>>(filename);
    } else if (type == "short") {
        return std::make_shared<RawFileReader<int16_t>>(filename);
    } else if (type == "ushort") {
        return std::make_shared<RawFileReader<uint16_t>>(filename);
    } else if (type == "float") {
        return std::make_shared<RawFileReader<float>>(filename);
    } else if (type == "wav") {
        return std::make_shared<WAVFileReader>(filename);
    }
    throw std::invalid_argument("Invalid FileReader type");
}
