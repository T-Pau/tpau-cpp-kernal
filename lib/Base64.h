#ifndef HAD_TPAU_CPP_KERNAL_BASE64_H
#define HAD_TPAU_CPP_KERNAL_BASE64_H

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

#include <cstdint>
#include <ostream>

namespace tpau::cpp_kernal {

/**
 * Base class to encode data in Base64.
 */
class Base64Encoder {
  public:
    virtual ~Base64Encoder() = default;

    /**
     * Encode the given data.
     *
     * @param data The data to encode.
     */
    void encode(const std::string& data);

    /**
     * Encode the given byte.
     *
     * @param datum The byte to encode.
     */
    void encode(uint8_t datum);

  protected:
    /**
     * Finish the encoding process.
     */
    void finish();

    /**
     * Output a character.
     *
     * @param c The character to output.
     */
    virtual void output(char c) = 0;

  private:
    /// Whether the encoding process has been finished.
    bool ended{false};

    /// The current position in the 4-character Base64 block.
    int position{0};

    /// The remaining bits from the previous byte that haven't been output yet.
    uint8_t remaining_bits{0};

    /**
     * Return the Base64 character corresponding to the given value.
     *
     * @param value The value to convert. Must be between 0 and 63.
     * @return The corresponding Base64 character.
     * @throws Exception if the value is out of range.
     */
    static char character(uint8_t value);

    /**
     * Output the Base64 character corresponding to the given value.
     *
     * @param value The value to convert and output. Must be between 0 and 63.
     * @throws Exception if the value is out of range.
     */
    void add(uint8_t value);
};

/**
 * Class to encode data in Base64 into a string.
 */
class Base64StringEncoder : public Base64Encoder {
  public:
    /**
     * Finish the encoding process and return the encoded data.
     *
     * @return The encoded data.
     */
    std::string end() {
        finish();
        return encoded_data;
    }

  protected:
    void output(char c) override { encoded_data += c; }

  private:
    std::string encoded_data;
};

/**
 * Class to encode data in Base64 to a stream.
 */
class Base64StreamEncoder : public Base64Encoder {
  public:
    /**
     * Create a Base64StreamEncoder.
     *
     * @param stream The output stream.
     * @param line_length The maximum line length, `0` for no limit.
     * @param indent The number of spaces to indent continuation lines.
     */
    Base64StreamEncoder(std::ostream& stream, size_t line_length = 0, size_t indent = 0) : stream{stream}, line_length{line_length}, indent{std::string(indent, ' ')} {}

    /**
     * Finish the encoding process.
     */
    void end() { finish(); }

  protected:
    void output(char c) override;

  private:
    /// The output stream to encode to.
    std::ostream& stream;

    /// The maximum line length. If 0, no line breaks are inserted.
    size_t line_length;

    /// The string to use for indenting continuation lines.
    std::string indent;

    /// The current position in the line.
    size_t line_position{0};
};

/**
 * Class to decode data from Base64.
 */
class Base64Decoder {
  public:
    /**
     * Decode the given data.
     *
     * @param data The data to decode. Whitespace characters are ignored.
     */
    void decode(const std::string& data);

    /**
     * Decode the given character.
     *
     * @param character The character to decode. Whitespace characters are ignored.
     */
    void decode(char character);

    /**
     * Finish the decoding process and return the decoded data.
     *
     * @return The decoded data.
     */
    const std::string& end() const;

  private:
    /// The decoded data.
    std::string decoded_string;

    /// Whether the end of the data has been reached.
    bool ended{false};

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
    static uint8_t value(char character);
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_BASE64_H
