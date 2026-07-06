#ifndef HAD_TPAU_CPP_KERNAL_UTF8_H
#define HAD_TPAU_CPP_KERNAL_UTF8_H

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
#include <string>

namespace tpau::cpp_kernal {

/**
  * Utility functions for working with UTF-8 and Unicode code-points.
  */
class UTF8 {
  public:
    /**
      * Decodes a UTF-8 string into a sequence of Unicode code-points.
      * 
      * @param string The UTF-8 string to decode.
      * @return A sequence of Unicode code-points.
      */
    static std::u32string decode(const std::string& string);

    /**
      * Encodes a Unicode code-point into a UTF-8 string.
      * 
      * @param codepoint The Unicode code-point to encode.
      * @param escape_non_printable If true, non-printable characters will be escaped as `\uXXXX`.
      * @return The UTF-8 encoded string.
      */
    static std::string encode(char32_t codepoint, bool escape_non_printable = false);

    /**
      * Encodes a sequence of Unicode code-points into a UTF-8 string.
      * 
      * @param string The sequence of Unicode code-points to encode.
      * @return The UTF-8 encoded string.
      */
    static std::string encode(const std::u32string& string);

    /**
      * Checks if a Unicode code-point is printable.
      * 
      * @param codepoint The Unicode code-point to check.
      * @return True if the code-point is printable, false otherwise.
      */
    static bool is_printable(char32_t codepoint);

  private:
    class FirstByte {
      public:
        FirstByte(uint8_t prefix, uint8_t mask, char32_t max_char): prefix{prefix}, mask{mask}, max_char{max_char} {}

        uint8_t prefix;
        uint8_t mask;
        char32_t max_char;
    };

    static void encode(char32_t codepoint, std::string& string);

    static const int utf8_continuation_bytes[];
    static const FirstByte first_bytes[];
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_UTF8_H
