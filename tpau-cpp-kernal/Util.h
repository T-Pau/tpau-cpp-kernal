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
#include <functional>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

#include "Symbol.h"

namespace tpau::cpp_kernal {

/**
 * Get a default output file name based on input file name and extension.
 *
 * It discards the directory part of the input file name and replaces the extension.
 *
 * @param input_filename The input file name.
 * @param extension The extension to use for the output file name.
 * @return The default output file name.
 */
std::filesystem::path default_output_filename(const std::filesystem::path& input_filename, std::string_view extension);

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
template <std::ranges::input_range R, typename T = std::ranges::range_value_t<R>, class Comp = std::less<T>> std::vector<T> sorted(const R& collection, Comp comp = {}) {
    auto sorted_collection = std::vector<T>();
    if constexpr (std::ranges::sized_range<R>) {
        sorted_collection.reserve(std::ranges::size(collection));
    }
    for (const auto& value : collection) {
        sorted_collection.emplace_back(value);
    }
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
std::string strerror_string(std::optional<int> errnum = {});

/**
 * Split a string into a vector of strings based on a set of delimiters.
 *
 * @param str The string to split.
 * @param delimiters The set of delimiters to use for splitting. (default: whitespace)
 * @param keep_empty If `true`, consecutive delimiters will result in empty strings in the output. (default: `false`)
 * @return The vector of split strings.
 */
std::vector<std::string> split(std::string_view str, std::string_view delimiters = " \t\n\r\f\v", bool keep_empty = false);

/**
 * Trim whitespace from the beginning and end of a string.
 *
 * @param str The string to trim.
 * @param whitespace The set of whitespace characters to trim. (default: whitespace)
 * @return The trimmed string.
 */
std::string_view trim(std::string_view str, std::string_view whitespace = " \t\n\r\f\v");

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_UTIL_H
