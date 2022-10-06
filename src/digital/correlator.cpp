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

#include "correlator.h"
#include <stdexcept>
#include <memory.h>
#include <iostream>

using namespace SatHelper;

Correlator::Correlator() {
    wordNumber = 0;
    currentWordSize = 0;
    wordsPtr = NULL;
    updatePointers();
}

Correlator::~Correlator() {
    if (wordsPtr != NULL) {
        delete[] wordsPtr;
    }
}

void Correlator::addWord(uint32_t word) {
    if (currentWordSize != 0 && currentWordSize != 32) {
        throw std::runtime_error("Bad");
    }
    VecU8 wordVec;

    for (int i = 0; i < 32; i++) {
        wordVec.push_back((word >> (32 - i - 1)) & 1 ? 0xFF : 0x00);
    }

    words.push_back(wordVec);
    correlation.push_back(0);
    position.push_back(0);
    tmpCorrelation.push_back(0);
    updatePointers();
}

void Correlator::addWord(uint64_t word) {
    if (currentWordSize != 0 && currentWordSize != 64) {
        throw std::runtime_error("Bad*2");
    }
    VecU8 wordVec;

    for (int i = 0; i < 64; i++) {
        wordVec.push_back((word >> (64 - i - 1) & 1) ? 0xFF : 0x00);
    }

    words.push_back(wordVec);
    correlation.push_back(0);
    position.push_back(0);
    tmpCorrelation.push_back(0);
    updatePointers();
}

void Correlator::resetCorrelation() {
    int numWords = words.size();
    if (words.size() > 0) {
        memset(correlationPtr, 0x00, numWords * sizeof(uint32_t));
        memset(positionPtr, 0x00, numWords * sizeof(uint32_t));
        memset(tmpCorrelationPtr, 0x00, numWords * sizeof(uint32_t));
    }
}

void Correlator::updatePointers() {
    // Cache Start of the array, so the loop doesn't call vector[]
    if (wordsPtr != NULL) {
        delete[] wordsPtr;
    }

    if (words.size() > 0) {
        wordsPtr = new uint8_t*[words.size()];
        for (unsigned int i = 0; i < words.size(); i++) {
            wordsPtr[i] = &words[i][0];
        }

        if (tmpCorrelation.size() > 0) {
            tmpCorrelationPtr = &tmpCorrelation[0];
        }

        if (correlation.size() > 0) {
            correlationPtr = &correlation[0];
        }
        if (position.size() > 0) {
            positionPtr = &position[0];
        }
    }
}

uint32_t _mm_sum_epu8(const __m128i v) {
    __m128i vsum = _mm_sad_epu8(v, _mm_setzero_si128());
    return _mm_extract_epi16(vsum, 0) + _mm_extract_epi16(vsum, 4);
}

void Correlator::correlate(uint8_t *data, uint32_t length) {
    int wordSize = words[0].size();
    int numWords = words.size();
    int maxSearch = length - wordSize;
    resetCorrelation();

    for (int i = 0; i < maxSearch; i++) {
        memset(tmpCorrelationPtr, 0, numWords * sizeof(uint32_t));

        for (int n = 0; n < numWords; n++) {
            __m128i sum = _mm_set1_epi8(0);
            for (int k = 0; k < wordSize/16; k++) {
                const __m128i _words = _mm_loadu_si128((__m128i_u *)&wordsPtr[n][k*16]);
                const __m128i _data  = _mm_loadu_si128((__m128i_u *)&data[i + k*16]);
                sum = _mm_add_epi8(sum, Correlator::hardCorrelate(_data, _words));
            }
        
            // https://stackoverflow.com/a/36998778
            __m128i vsum = _mm_sad_epu8(sum, _mm_setzero_si128());
            tmpCorrelationPtr[n] = _mm_cvtsi128_si32(vsum) + _mm_extract_epi16(vsum, 4);
        }

        for (int n = 0; n < numWords; n++) {
            if (tmpCorrelationPtr[n] > correlationPtr[n]) {
                correlationPtr[n] = tmpCorrelationPtr[n];
                positionPtr[n] = i;
            }
        }
    }

    uint32_t corr = 0;
    for (int n = 0; n < numWords; n++) {
        if (correlationPtr[n] > corr) {
            wordNumber = n;
            corr = correlationPtr[n];
        }
    }
}
