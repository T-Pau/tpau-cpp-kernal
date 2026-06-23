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

#include "FileReader.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <ranges>

#include "Exception.h"
#include "Util.h"

namespace tpau::cpp_kernal {

FileReader FileReader::global;

std::vector<std::string> FileReader::empty_file;


const std::vector<std::string>& FileReader::read(Symbol file_name, bool optional) {
    const auto it = files.find(file_name);
    if (it != files.end()) {
        return it->second;
    }

    std::ifstream input_file(file_name.str());
    if (input_file) {
        std::vector<std::string> lines;
        std::string s;
        while (getline(input_file, s)) {
            lines.push_back(s);
        }

        files[file_name] = std::move(lines);
        return files[file_name];
    }
    else if (errno == EEXIST && optional) {
        return empty_file;
    }
    else {
        throw Exception("can't open '%s': %s", file_name.c_str(), strerror_string(errno).c_str());
    }
}


std::string FileReader::read_binary(Symbol file_name, bool optional) {
    if (std::ifstream input_file{file_name.str()}) {
        binary_files.insert(file_name);
        return std::string{std::istreambuf_iterator<char>{input_file}, {}};
    }
    else if (errno == EEXIST && optional) {
        return "";
    }
    else {
        throw Exception("can't open '%s': %s", file_name.c_str(), strerror_string(errno).c_str());
    }
}


const std::string& FileReader::get_line(Symbol file, size_t line_number) const {
    const auto it = files.find(file);

    if (it == files.end()) {
        throw Exception("unknown file '%s'", file.c_str());
    }

    if (line_number == 0 || line_number > it->second.size()) {
        throw Exception("line integer %zu out of range in '%s'", line_number, file.c_str());
    }

    return it->second[line_number - 1];
}


std::vector<Symbol> FileReader::file_names() const {
    auto file_names = std::vector<Symbol>();

    for (const auto& name : std::views::keys(files)) {
        file_names.emplace_back(name);
    }
    for (const auto& name : binary_files) {
        file_names.emplace_back(name);
    }

    std::sort(file_names.begin(), file_names.end());

    return file_names;
}

} // namespace tpau::cpp_kernal
