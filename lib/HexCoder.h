#ifndef HAD_TPAU_CPP_KERNAL_HEX_CODER_H
#define HAD_TPAU_CPP_KERNAL_HEX_CODER_H

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

#include "Coder.h"

namespace tpau::cpp_kernal {

class HexEncoderEngine : public CoderEngine {
  public:
    HexEncoderEngine(CoderOutput& output);

  protected:
    void encode_implementation(uint8_t datum) override;
    void finish_implementation() override;

  private:
    CoderOutput& output;
};

class HexDecoderEngine : public CoderEngine {
  public:
    HexDecoderEngine(CoderOutput& output);

  protected:
    void encode_implementation(uint8_t datum) override;
    void finish_implementation() override;

  private:
    CoderOutput& output;
};

/**
 * Encode data in Hex and collect the encoded data in a string.
 */
class HexStringEncoder : public Coder {
  public:
    HexStringEncoder() : Coder(engine) {};

  private:
    StringCoderOutput output;
    HexEncoderEngine engine{output};
};

/**
 * Encode data in Hex and write it to a stream.
 */
class HexStreamEncoder : public Coder {
  public:
    HexStreamEncoder(std::ostream& stream, size_t line_length = 0, size_t indent = 0) : Coder(engine), output(stream, line_length, indent), engine(output) {}

  private:
    StreamCoderOutput output;
    HexEncoderEngine engine;
};

/**
 * Decode Hex-encoded data and collect the decoded data in a string.
 */
class HexStringDecoder : public Coder {
  public:
    HexStringDecoder() : Coder(engine) {};

  private:
    StringCoderOutput output;
    HexDecoderEngine engine{output};
};

/**
 * Decode Hex-encoded data and write the decoded data to a stream.
 */
class HexStreamDecoder : public Coder {
  public:
    HexStreamDecoder(std::ostream& stream, size_t line_length = 0, size_t indent = 0) : Coder(engine), output(stream, line_length, indent), engine(output) {}

  private:
    StreamCoderOutput output;
    HexDecoderEngine engine;
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_HEX_CODER_H
