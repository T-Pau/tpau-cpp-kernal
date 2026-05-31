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

#include "HexCoder.h"

#include "Exception.h"

namespace tpau::cpp_kernal {

void HexEncoderEngine::process_implementation(uint8_t datum) {
    write(encode(datum >> 4));
    write(encode(datum & 0xf));
}

char HexEncoderEngine::encode(uint8_t value) {
    if (value < 10) {
        return value + '0';
    }
    else if (value < 16) {
        return value - 10 + 'a';
    }
    else {
        throw Exception("internal error: encoding more than 4 bits in HexEncoder");
    }
}


std::string HexStringEncoder::end() {
    finish();
    return output.get_string();
}


void HexDecoderEngine::process_implementation(uint8_t datum) {
    if (datum == ' ' || datum == '\t' || datum == '\n' || datum == '\r') {
        return;
    }

    uint8_t bits = decode(static_cast<char>(datum));
    if (has_partial_byte) {
        write(partial_byte | bits);
        has_partial_byte = false;
    }
    else {
        partial_byte = bits << 4;
        has_partial_byte = true;
    }
}

void HexDecoderEngine::finish_implementation() {
    if (has_partial_byte) {
        throw Exception("incomplete hex data");
    }
}

uint8_t HexDecoderEngine::decode(char character) {
    if (character >= '0' && character <= '9') {
        return character - '0';
    }
    else if (character >= 'a' && character <= 'f') {
        return character - 'a' + 10;
    }
    else if (character >= 'A' && character <= 'F') {
        return character - 'A' + 10;
    }
    else {
        throw Exception("invalid character in hex data");
    }
}

std::string HexStringDecoder::end() {
    finish();
    return output.get_string();
}


} // namespace tpau::cpp_kernal
