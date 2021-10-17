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

template<typename A, typename B>
class Block {
    public:
        Block(size_t request = BUFFER_SIZE, size_t size = BUFFER_SIZE*10) 
            : in(request),
              out(BUFFER_SIZE) {
            if (!std::is_same<B, Empty>::value) {
                out_pipe = std::make_shared<Pipe<B>>(size);
            }
        };

        void start() {
            if (!std::is_same<A, Empty>::value & !std::is_same<B, Empty>::value) {
                // Input and output
                thread = std::make_shared<std::thread>([this]() {
                    while (running) {
                        if (in_pipe->pop(in.data(), in.size()) != 0) {
                            size_t n = work(in.data(), out.data(), in.size());
                            out_pipe->push(out.data(), n);
                        }
                    }
                });
            } else if (!std::is_same<B, Empty>::value){
                // Output only
                thread = std::make_shared<std::thread>([this]() {
                    while (running) {
                        size_t n = work(out.data(), out.size());
                        out_pipe->push(out.data(), n);
                    }
                });
            } else if (!std::is_same<A, Empty>::value) {
                // Input only
                thread = std::make_shared<std::thread>([this]() {
                    while (running) {
                        if (in_pipe->pop(in.data(), in.size()) != 0) {
                            work(in.data(), in.size());
                        }
                    }
                });
            } else {
                throw std::runtime_error("Cannot have block without an input or output");
            }
        }

        void set_runvar(bool &run) {
            running = run;
        }
        void set_running(bool run) {
            _running = run;
            running = _running;
        }

        // This is bad, dont do this
        std::shared_ptr<Pipe<B>> out_pipe;
        std::shared_ptr<Pipe<A>> in_pipe;
        std::vector<A> in;
        std::vector<B> out;

    private:
        std::shared_ptr<std::thread> thread;
        bool _running = true;
        bool &running = _running;

        // Virtual work functions
        virtual size_t work([[maybe_unused]] const A *in, [[maybe_unused]] B *out, [[maybe_unused]] size_t n) { throw std::runtime_error("No matching function with signature size_t(const A* in, B* out, size_t n"); };
        virtual size_t work([[maybe_unused]] B *out, [[maybe_unused]] size_t n)                               { throw std::runtime_error("No matching function with signature size_t(B* out, size_n"); };
        virtual void work([[maybe_unused]] const A *in, [[maybe_unused]] size_t n)                            { throw std::runtime_error("No matching function with signature void(const A* in, size_t n"); };
};

#endif
