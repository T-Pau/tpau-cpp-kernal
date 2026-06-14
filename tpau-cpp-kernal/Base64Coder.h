#ifndef HAD_TPAU_CPP_KERNAL_BASE64_CODER_H
#define HAD_TPAU_CPP_KERNAL_BASE64_CODER_H

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

class Base64EncoderEngine : public CoderEngine {
  public:
    Base64EncoderEngine(CoderOutput& output) : CoderEngine(output) {}

  protected:
    void process_implementation(uint8_t datum) override;
    void finish_implementation() override;

  private:
    /// The current position in the 4-character Base64 block.
    int position{0};

    /// The remaining bits from the previous byte that haven't been output yet.
    uint8_t remaining_bits{};

    /**
     * Return the value corresponding to the given Base64 character.
     *
     * @param character The Base64 character.
     * @return The corresponding value.
     * @throws Exception if the character is invalid.
     */
    static char encode(uint8_t value);
};

class Base64DecoderEngine : public CoderEngine {
  public:
    Base64DecoderEngine(CoderOutput& output) : CoderEngine(output) {}

  protected:
    void process_implementation(uint8_t datum) override;
    void finish_implementation() override;

  private:
    /// The current position in the 4-character Base64 block.
    int position{0};

    /// The remaining bits from the previous byte that haven't been output yet.
    uint8_t remaining_bits{};

    /// Whether the end marker (`=`) has been seen in the input.
    bool end_marker_seen{false};

    /**
     * Return the value corresponding to the given Base64 character.
     *
     * @param character The Base64 character.
     * @return The corresponding value.
     * @throws Exception if the character is invalid.
     */
    static uint8_t decode(char character);
};

/**
 * Encode data in Base64 and collect the encoded data in a string.
 */
class Base64StringEncoder : public Coder {
  public:
    Base64StringEncoder() : Coder(engine) {};

    /**
     * Finish the encoding process and return the encoded data.
     *
     * @return The encoded data.
     */
    std::string end();

  private:
    StringCoderOutput output;
    Base64EncoderEngine engine{output};
};

/**
 * Encode data in Base64 and write it to a stream.
 */
class Base64StreamEncoder : public Coder {
  public:
    Base64StreamEncoder(std::ostream& stream, size_t line_length = 0, size_t indent = 0) : Coder(engine), output(stream, line_length, indent), engine(output) {}

    void end() { finish(); }

  private:
    StreamCoderOutput output;
    Base64EncoderEngine engine;
};

/**
 * Decode Base64-encoded data and collect the decoded data in a string.
 */
class Base64StringDecoder : public Coder {
  public:
    Base64StringDecoder() : Coder(engine) {};

    std::string end();

  protected:
    StringCoderOutput output;
    Base64DecoderEngine engine{output};
};

/**
 * Decode Base64-encoded data and write the decoded data to a stream.
 */
class Base64StreamDecoder : public Coder {
  public:
    Base64StreamDecoder(std::ostream& stream, size_t line_length = 0, size_t indent = 0) : Coder(engine), output(stream, line_length, indent), engine(output) {}

  private:
    StreamCoderOutput output;
    Base64DecoderEngine engine;
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_BASE64_CODER_H
