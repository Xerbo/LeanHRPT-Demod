/*
 * Taken from https://github.com/opensatelliteproject/libsathelper
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

#ifndef INCLUDES_PACKETFIXER_H_
#define INCLUDES_PACKETFIXER_H_

#include <mutex>


// // cosf(90.f * M_PI / 180.f);
// #define PacketFixer_C 0.f
// // sinf(90.f * M_PI / 180.f);
// #define PacketFixer_D 1.f

namespace SatHelper {
    enum PhaseShift {
        DEG_0, DEG_90, DEG_180, DEG_270
    };

    class PacketFixer {
        private:

          // static inline float S8toFloat(uint8_t v) {
          //   return ((int8_t)v) / 127.f;
          // }

          // static inline uint8_t FloatToS8(float v) {
          //   return (uint8_t) ((v + 128) * 127);
          // }

          // static inline void Rotate(uint8_t *data) {
          //   float a = S8toFloat(data[0]), b = S8toFloat(data[1]);
          //   data[0] = FloatToS8(a * PacketFixer_C - b * PacketFixer_D);
          //   data[1] = FloatToS8(a * PacketFixer_D + b * PacketFixer_C);
          // }
          static inline void Rotate(uint8_t *data) {
            int8_t a = data[0];
            int8_t b = data[1];
            data[0] = (128 - b);
            data[1] = a + 128;
          }

        public:
          void fixPacket(uint8_t *data, uint32_t length, SatHelper::PhaseShift phaseShift, bool iqInversion);
    };
}

#endif /* INCLUDES_PACKETFIXER_H_ */
