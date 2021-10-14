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

#ifndef DSP_BLOCK_H
#define DSP_BLOCK_H

const size_t BUFFER_SIZE = 8192;

#include "util/pipe.hh"
#include <thread>
#include <stdexcept>
#include <vector>

using complex = std::complex<float>;

// Used to specify that an input (or output) is not used
struct Empty {
    int dummy;
};

#include <iostream>

template<typename A, typename B>
class Block {
    public:
        Block(size_t size = BUFFER_SIZE*10) {
            if (!std::is_same<B, Empty>::value) {
                out_pipe = std::make_shared<Pipe<B>>(size);
            }
        };

        void start() {
            if (!std::is_same<A, Empty>::value & !std::is_same<B, Empty>::value) {
                // Input and output
                thread = std::make_shared<std::thread>([this]() {
                    std::vector<A> in(BUFFER_SIZE);
                    std::vector<B> out(BUFFER_SIZE);
                    while (running) {
                        if (in_pipe->pop(in.data(), BUFFER_SIZE) != 0) {
                            size_t n = work(in.data(), out.data(), BUFFER_SIZE);
                            out_pipe->push(out.data(), n);
                        }
                    }
                });
            } else if (!std::is_same<B, Empty>::value){
                // Output only
                thread = std::make_shared<std::thread>([this]() {
                    std::vector<B> out(BUFFER_SIZE);
                    while (running) {
                        size_t n = work(out.data(), BUFFER_SIZE);
                        out_pipe->push(out.data(), n);
                    }
                });
            } else if (!std::is_same<A, Empty>::value) {
                // Input only
                thread = std::make_shared<std::thread>([this]() {
                    std::vector<A> in(BUFFER_SIZE);
                    while (running) {
                        if (in_pipe->pop(in.data(), BUFFER_SIZE) != 0) {
                            work(in.data(), BUFFER_SIZE);
                        }
                    }
                });
            } else {
                throw std::runtime_error("Cannot have block without an input or output");
            }
        }

        // This is bad, dont do this
        std::shared_ptr<Pipe<B>> out_pipe;
        std::shared_ptr<Pipe<A>> in_pipe;

        bool running = true;
    private:
        std::shared_ptr<std::thread> thread;

        // Virtual work functions
        virtual size_t work([[maybe_unused]] const A *in, [[maybe_unused]] B *out, [[maybe_unused]] size_t n) { throw std::runtime_error("override damn it"); };
        virtual size_t work([[maybe_unused]] B *out, [[maybe_unused]] size_t n)                               { throw std::runtime_error("override damn it"); };
        virtual void work([[maybe_unused]] const A *in, [[maybe_unused]] size_t n)                            { throw std::runtime_error("override damn it"); };
};

#endif
