#ifndef HAD_TPAU_CPP_KERNAL_INT_H
#define HAD_TPAU_CPP_KERNAL_INT_H

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
  * This class provides static methods for various integer utility functions.
  */
class Int {
  public:
    /**
     * Encode the given signed integer value into a string of bytes.
     * 
     * @param bytes The string to append the encoded bytes to.
     * @param value The signed integer value to encode.
     * @param size The number of bytes to use for the encoding. If 0, the minimum number of bytes required to represent the value will be used.
     * @param byte_order The byte order to use for the encoding.
     */
    static void encode(std::string& bytes, int64_t value, uint64_t size, uint64_t byte_order) { encode(bytes, static_cast<uint64_t>(value), size > 0 ? size : minimum_byte_size(value), byte_order); }

    /**
     * Encode the given unsigned integer value into a string of bytes.
     * 
     * @param bytes The string to append the encoded bytes to.
     * @param value The unsigned integer value to encode.
     * @param size The number of bytes to use for the encoding. If 0, the minimum number of bytes required to represent the value will be used.
     * @param byte_order The byte order to use for the encoding.
     */
    static void encode(std::string& bytes, uint64_t value, uint64_t size, uint64_t byte_order);

    /**
     * Get the minimum number of bytes required to represent the given signed integer value.
     * 
     * @param value The signed integer value.
     * @return The minimum number of bytes required to represent the value.
     */
    static size_t minimum_byte_size(int64_t value);

    /**
     * Get the minimum number of bytes required to represent the given unsigned integer value.
     * 
     * @param value The unsigned integer value.
     * @return The minimum number of bytes required to represent the value.
     */
    static size_t minimum_byte_size(uint64_t value);

    /**
     * Align the given value to the specified alignment.
     * 
     * @param value The value to align.
     * @param alignment The alignment boundary.
     * @return The aligned value.
     */
    static size_t align(size_t value, size_t alignment);
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_INT_H
