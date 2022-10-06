/*
 * Adapted from https://github.com/opensatelliteproject/libsathelper
 *
 * Orignal license is as follows:
 *  Copyright 2016 Lucas Teske
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of
 *  this software and associated documentation files (the "Software"), to deal in
 *  the Software without restriction, including without limitation the rights to
 *  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *  of the Software, and to permit persons to whom the Software is furnished to do
 *  so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#ifndef INCLUDES_CORRELATOR_H_
#define INCLUDES_CORRELATOR_H_

#include "util/sse.h"
#include <vector>
#include <cstdint>
#include <stdint.h>
#include <cstdio>

// a >= b
#define _mm_cmpge_epu8(a, b) _mm_cmpeq_epi8(_mm_max_epu8(a, b), a)
// b >= a
#define _mm_cmple_epu8(a, b) _mm_cmpge_epu8(b, a)

namespace SatHelper {

    typedef std::vector<uint8_t> VecU8;

    class Correlator {
    private:
        std::vector<VecU8> words;
        std::vector<uint32_t> correlation;
        std::vector<uint32_t> tmpCorrelation;
        std::vector<uint32_t> position;
        uint32_t wordNumber;
        uint8_t currentWordSize;

        // Cache Start of the array, so the loop doesn't call vector[]
        uint8_t **wordsPtr;
        uint32_t *tmpCorrelationPtr;
        uint32_t *correlationPtr;
        uint32_t *positionPtr;

        void resetCorrelation();
        void updatePointers();
    public:
        // Undefined behaviour if `wordByte` is anything else than 0 or 255
        static inline __m128i hardCorrelate(__m128i dataByte, __m128i wordByte) {
            const __m128i midpoint = _mm_set1_epi8(127-1);
            const __m128i mask = _mm_set1_epi8(1);

            __m128i a = _mm_cmple_epu8(dataByte, midpoint);
            return _mm_and_si128(_mm_cmpeq_epi8(a, wordByte), mask);
        }

        Correlator();
        ~Correlator();

        inline uint32_t getHighestCorrelation() {
            return correlation[wordNumber];
        }

        inline uint32_t getHighestCorrelationPosition() {
            return position[wordNumber];
        }

        inline uint32_t getCorrelationWordNumber() {
            return wordNumber;
        }

        inline size_t getCorrelationWordCount() {
            return words.size();
        }

        void addWord(uint32_t word);
        void addWord(uint64_t word);

        void correlate(uint8_t *data, uint32_t length);
    };

}

#endif /* INCLUDES_CORRELATOR_H_ */
