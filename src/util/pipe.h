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

#ifndef UTIL_PIPE_H
#define UTIL_PIPE_H

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

template <typename T>
class Pipe {
    public:
        Pipe(size_t size) : d_max_size(size) {
#ifdef _WIN32
            if (_pipe(handles, size*sizeof(T), 'b') != -1) {
#else
            if (pipe(handles) == -1) {
#endif
                throw std::runtime_error("Error creating pipe");
            }
        }
        ~Pipe() {
            if (handles[0]) close(handles[0]);
            if (handles[1]) close(handles[1]);
        }

        void push(T *ptr, size_t n) {
            while (d_size + n > d_max_size) {
                asm("nop");
            }

            ssize_t res = write(handles[1], ptr, n*sizeof(T));
            if (res != -1) {
                d_size += n;
            }
        }

        size_t pop(T *ptr, size_t n) {
            ssize_t res = read(handles[0], ptr, n*sizeof(T));
            if (res != -1) {
                d_size -= n;
                return n;
            }

            return 0;
        }
    private:
        // Handles
        int handles[2];

        // Size
        const size_t d_max_size;
        size_t d_size = 0;
};

#endif
