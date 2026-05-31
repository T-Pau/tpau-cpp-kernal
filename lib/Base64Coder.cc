/*
Copyright (C) Dieter Baron

The authors can be contacted at <tpau-cpp-kernal@tpau.group>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. The names of the authors may not be used to endorse or promote
   products derived from this software without specific prior
   written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "Base64Coder.h"

void Base64EncoderEngine::encode_implementation(uint8_t datum) {
    uint8_t value;

    switch (position) {
    case 0:
        value = datum >> 2;
        remaining_bits = (datum & 0x3) << 4;
        break;

    case 1:
        value = remaining_bits | (datum >> 4);
        remaining_bits = (datum & 0xf) << 2;
        break;

    case 2:
        value = remaining_bits | (datum >> 6);
        remaining_bits = datum & 0x3f;
        break;

    default:
        throw Exception("internal error: invalid position in Base64Encoder");
    }

    position = (position + 1) % 4;
    output(value);
    if (position == 0) {
        output(remaining_bits);
        remaining_bits = 0;
    }
}

void Base64EncoderEngine::finish_implementation() {
    if (position == 0) {
        return;
    }
    output(remaining_bits);
    while (position < 3) {
        output('=');
        position += 1;
    }
}