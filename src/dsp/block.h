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

#include "util/pipe.hh"

#include <stdexcept>
#include <vector>
#include <fstream>
#include <complex>
#include <cstddef>
#include <thread>
#include <atomic>
#include <QThread>

const size_t BUFFER_SIZE = 8192;

using complex = std::complex<float>;

// Used to specify that an input (or output) is not used
struct Empty {
    int dummy;
};

class QtThread : public QThread {
    public:
        QtThread(std::function<void()> x)
            : function(x) { }

        void run() override {
            function();
        }
    private:
        std::function<void()> function;
};

class BlockInterface {
    public:
        virtual void start()=0;
        virtual void stop()=0;
};

template<typename A, typename B>
class Block : public BlockInterface {
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
                thread = std::make_shared<QtThread>([this]() {
                    while (running) {
                        if (in_pipe->pop(in.data(), in.size()) != 0) {
                            size_t n = work(in.data(), out.data(), in.size());
                            out_pipe->push(out.data(), n);
                        }
                    }
                });
            } else if (!std::is_same<B, Empty>::value){
                // Output only
                thread = std::make_shared<QtThread>([this]() {
                    while (running) {
                        size_t n = work(out.data(), out.size());
                        out_pipe->push(out.data(), n);
                    }
                });
            } else if (!std::is_same<A, Empty>::value) {
                // Input only
                thread = std::make_shared<QtThread>([this]() {
                    while (running) {
                        if (in_pipe->pop(in.data(), in.size()) != 0) {
                            work(in.data(), in.size());
                        }
                    }
                });
            } else {
                throw std::runtime_error("Cannot have block without an input or output");
            }

            running = true;
            thread->start();
            thread->setPriority(QThread::HighestPriority);
        }
        void stop() {
            running = false;

            // Give it some time to respond gracefully
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            // Kick it up the ass if it doesn't
            if (thread->isRunning()) {
                thread->terminate();
                thread->wait();
            }
        }

        // This is bad, dont do this
        std::shared_ptr<Pipe<B>> out_pipe;
        std::shared_ptr<Pipe<A>> in_pipe;
        std::vector<A> in;
        std::vector<B> out;

    private:
        std::shared_ptr<QtThread> thread;
        std::atomic<bool> running;

        // Virtual work functions
        virtual size_t work([[maybe_unused]] const A *in, [[maybe_unused]] B *out, [[maybe_unused]] size_t n) { throw std::runtime_error("No matching function with signature size_t(const A* in, B* out, size_t n"); };
        virtual size_t work([[maybe_unused]] B *out, [[maybe_unused]] size_t n)                               { throw std::runtime_error("No matching function with signature size_t(B* out, size_n"); };
        virtual void work([[maybe_unused]] const A *in, [[maybe_unused]] size_t n)                            { throw std::runtime_error("No matching function with signature void(const A* in, size_t n"); };
};

// Block where both pipes point to the same address in memory
// to simply passthrough the pipe pointer
template<typename T>
class Passthrough : public BlockInterface {
    public:
        Passthrough(...) : out_pipe(in_pipe) { }
        void start() {}
        void stop() {}
        std::shared_ptr<Pipe<T>> &out_pipe;
        std::shared_ptr<Pipe<T>> in_pipe;
};

#endif
