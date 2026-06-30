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

#include <algorithm>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "Symbol.h"

namespace tpau::cpp_kernal {

/**
 * Join a vector of symbols into a string with a separator between the symbols.
 *
 * @param symbols The symbols to join.
 * @param separator The separator to use between the symbols. (default: ", ")
 * @return The joined string.
 */
std::string join(const std::vector<Symbol>& symbols, std::string_view separator = ", ");

/**
 * Join a range of strings into a string with a separator between the strings.
 *
 * @tparam R The type of the range.
 * @param strings The range of strings to join.
 * @param separator The separator to use between the strings. (default: ", ")
 * @return The joined string.
 */
template <std::ranges::input_range R>
    requires std::same_as<std::iter_value_t<R>, std::string>
std::string join(const R& strings, std::string_view separator) {
    auto s = std::string();
    auto first = true;

    for (auto& str : strings) {
        if (first) {
            first = false;
        }
        else {
            s += separator;
        }
        s += str;
    }

    return s;
}

/**
 * Sort a range of elements and return the sorted collection.
 *
 * @tparam R The type of the range.
 * @param collection The range of elements to sort.
 * @param comp The comparison function to use for sorting. (default: std::less)
 * @return The sorted collection.
 */
template <std::ranges::input_range R, typename T = std::iter_value_t<R>, class Comp = std::less<T>> std::vector<T> sorted(const R& collection, Comp comp = {}) {
    auto sorted_collection = std::vector<T>(collection.begin(), collection.end());
    std::sort(sorted_collection.begin(), sorted_collection.end(), comp);
    return sorted_collection;
}

/**
 * Replace the extension of a file name with a new extension.
 *
 * @param file_name The file name to replace the extension of.
 * @param extension The new extension to use.
 * @return The file name with the replaced extension.
 */
std::filesystem::path replace_extension(const std::filesystem::path& file_name, std::string_view extension);

/**
 * Get a string representation of a system error code.
 *
 * @param errnum The error code to get the string representation of. If not provided, the current value of `errno` will be used.
 * @return The string representation of the error code.
 */
std::string strerror_string(std::optional<int> errnum);

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_UTIL_H
