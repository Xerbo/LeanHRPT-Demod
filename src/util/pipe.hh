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

#include "pipe.h"

template<typename T>
class Pipe {
    public:
        Pipe(size_t n) {
            pipe_t *pipe = pipe_new(sizeof(T), n);
            producer = pipe_producer_new(pipe);
            consumer = pipe_consumer_new(pipe);
            pipe_free(pipe);
        }
        ~Pipe() {
            //pipe_producer_free(producer);
            //pipe_consumer_free(consumer);
        }
        size_t pop(T *ptr, size_t n) {
            return pipe_pop(consumer, ptr, n);
        }
        void push(T* ptr, size_t n) {
            return pipe_push(producer, ptr, n);
        }
    private:
        pipe_producer_t *producer;
        pipe_consumer_t *consumer;
};

#endif
