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

#include "Exception.h"

namespace tpau::cpp_kernal {

void Base64EncoderEngine::process_implementation(uint8_t datum) {
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

    output.write(encode(value));
    position += 1;
    if (position == 3) {
        output.write(encode(remaining_bits));
        remaining_bits = 0;
        position = 0;
    }
}

void Base64EncoderEngine::finish_implementation() {
    if (position == 0) {
        return;
    }
    write(encode(remaining_bits));
    while (position < 3) {
        write('=');
        position += 1;
    }
}


char Base64EncoderEngine::encode(uint8_t value) {
    if (value < 26) {
        return value + 'A';
    }
    else if (value < 52) {
        return value - 26 + 'a';
    }
    else if (value < 62) {
        return value - 52 + '0';
    }
    else if (value == 62) {
        return '+';
    }
    else if (value == 63) {
        return '/';
    }
    else {
        throw Exception("internal error: encoding more than 6 bits in Base64Encoder");
    }
}

std::string Base64StringEncoder::end() {
    finish();
    return output.get_string();
}


void Base64DecoderEngine::process_implementation(uint8_t datum) {
    if (datum == ' ' || datum == '\t' || datum == '\n' || datum == '\r') {
        return;
    }

    if (datum == '=') {
        if (position == 0) {
            if (end_marker_seen) {
                throw Exception("garbage after end marker in base64 data");
            }
            else {
                throw Exception("unexpected end marker in base64 data");
            }
        }
        end_marker_seen = true;
        position = (position + 1) % 4;
        return;
    }

    if (end_marker_seen) {
        throw Exception("garbage after end marker in base64 data");
    }

    const auto bits = decode(datum);
    uint8_t value{};

    switch (position) {
    case 0:
        remaining_bits = bits << 2;
        break;

    case 1:
        value = remaining_bits | (bits >> 4);
        remaining_bits = (bits & 0xf) << 4;
        break;

    case 2:
        value = remaining_bits | (bits >> 2);
        remaining_bits = (bits & 0x3) << 6;
        break;

    case 3:
        value = remaining_bits | bits;
        break;

    default:
        throw Exception("internal error: invalid position in Base64Decoder");
    }

    if (position != 0) {
        write(value);
    }
    position = (position + 1) % 4;
}

void Base64DecoderEngine::finish_implementation() {
    if (!end_marker_seen && position != 0) {
        throw Exception("incomplete base64 data");
    }
}

uint8_t Base64DecoderEngine::decode(char character) {
    if (character >= 'A' && character <= 'Z') {
        return character - 'A';
    }
    else if (character >= 'a' && character <= 'z') {
        return character - 'a' + 26;
    }
    else if (character >= '0' && character <= '9') {
        return character - '0' + 52;
    }
    else if (character == '+') {
        return 62;
    }
    else if (character == '/') {
        return 63;
    }
    else {
        throw Exception("invalid character in base64 data");
    }
}

std::string Base64StringDecoder::end() {
    finish();
    return output.get_string();
}


} // namespace tpau::cpp_kernal
