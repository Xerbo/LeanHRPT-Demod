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

#include "packetfixer.h"

using namespace SatHelper;

void PacketFixer::fixPacket(uint8_t *data, uint32_t length, SatHelper::PhaseShift phaseShift, bool iqInversion) {
  if (iqInversion || phaseShift != SatHelper::PhaseShift::DEG_0) {
    for (uint32_t i = 0; i < length; i+=2) {
      if (iqInversion) {
        uint8_t x = data[i+1];
        data[i+1] = data[i];
        data[i] = x;
      }
      if (phaseShift == SatHelper::PhaseShift::DEG_90 || phaseShift == SatHelper::PhaseShift::DEG_270) {
        Rotate(&data[i]);
      }
      if (phaseShift == SatHelper::PhaseShift::DEG_180 || phaseShift == SatHelper::PhaseShift::DEG_270) {
        data[i] ^= 0xFF;
        data[i+1] ^= 0xFF;
      }
    }
  }
}
