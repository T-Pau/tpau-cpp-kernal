#ifndef HAD_TPAU_CPP_KERNAL_UTIL_H
#define HAD_TPAU_CPP_KERNAL_UTIL_H

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

#include <cstdarg>
#include <filesystem>
#include <string>
#include <vector>

#include "Symbol.h"
#include "printf_like.h"

namespace tpau::cpp_kernal {

/**
 * Format a string using `printf`-style formatting.
 *
 * @param format The format string.
 * @param ... The arguments to format.
 * @return The formatted string.
 */
std::string string_format(const char* format, ...) PRINTF_LIKE(1, 2);

/**
 * Format a string using `printf`-style formatting with a `va_list`.
 *
 * @param format The format string.
 * @param ap The arguments to format.
 * @return The formatted string.
 */
std::string string_format_v(const char* format, va_list ap);

/**
 * Join a vector of symbols into a string with a separator between the symbols.
 *
 * @param symbols The symbols to join.
 * @param separator The separator to use between the symbols. (default: ", ")
 * @return The joined string.
 */
std::string join(const std::vector<Symbol>& symbols, const std::string& separator = ", ");

/**
 * Replace the extension of a file name with a new extension.
 *
 * @param file_name The file name to replace the extension of.
 * @param extension The new extension to use.
 * @return The file name with the replaced extension.
 */
std::filesystem::path replace_extension(const std::filesystem::path& file_name, const std::string& extension);

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_UTIL_H
