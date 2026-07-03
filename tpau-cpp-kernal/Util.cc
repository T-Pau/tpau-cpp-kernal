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

#include "Util.h"

#include <cstring>
#include <filesystem>
#include <format>

#include "compat.h"

namespace tpau::cpp_kernal {

std::filesystem::path default_output_filename(const std::filesystem::path& input_filename, std::string_view extension) { return replace_extension(input_filename.filename(), extension); }


std::filesystem::path replace_extension(const std::filesystem::path& file_name, std::string_view extension) { return file_name.parent_path() / (std::format("{}.{}", file_name.stem().string(), extension)); }


std::string join(const std::vector<Symbol>& symbols, std::string_view separator) {
    auto s = std::string();
    auto first = true;

    for (auto& symbol : symbols) {
        if (first) {
            first = false;
        }
        else {
            s += separator;
        }
        s += symbol.str();
    }

    return s;
}

std::string strerror_string(std::optional<int> errnum) {
    if (!errnum) {
        errnum = errno;
    }
#if defined(HAVE_STRERROR_S)
    char buf[strerrorlen_s(*errnum)];
    strerror_s(buf, sizeof(buf), *errnum);
    return std::string(buf);
#else
    return std::string(strerror(*errnum));
#endif
}

std::vector<std::string> split(std::string_view str, std::string_view delimiters, bool keep_empty) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = 0;

    while (end < str.size()) {
        if (delimiters.find(str[end]) != std::string_view::npos) {
            if (keep_empty || end > start) {
                result.emplace_back(str.substr(start, end - start));
            }
            start = end + 1;
        }
        end++;
    }

    if (keep_empty || end > start) {
        result.emplace_back(str.substr(start, end - start));
    }

    return result;
}

std::string_view trim(std::string_view str, std::string_view whitespace) {
    auto start = str.find_first_not_of(whitespace);
    if (start == std::string_view::npos) {
        return {};
    }
    auto end = str.find_last_not_of(whitespace);
    if (end == std::string_view::npos) {
        return str.substr(start);
    }
    return str.substr(start, end - start + 1);
}

} // namespace tpau::cpp_kernal
