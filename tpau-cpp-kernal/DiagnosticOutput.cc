/*
Copyright (C) Dieter Baron

The authors can be contacted at <tpau-cpp-kernal@tpau.group>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   note, this list of conditions and the following disclaimer.

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

#include "DiagnosticOutput.h"

#include "FileReader.h"

namespace tpau::cpp_kernal {

DiagnosticOutput DiagnosticOutput::global;

const char* DiagnosticOutput::diagnostics_severity_name(Severity severity) {
    switch (severity) {
    case NOTE:
        return "note";

    case WARNING:
        return "warning";

    case ERROR:
        return "error";
    }

    throw Exception("invalid severity {}", static_cast<int>(severity));
}

void DiagnosticOutput::output(Symbol category, Severity severity, const Location& location, std::string_view message) {
    if (ignored_categories.contains(category)) {
        return;
    }
    if (severity >= fail_severity) {
        fail_flag = true;
    }
    if (!location.empty()) {
        diagnostics_file << location << ": ";
    }
    diagnostics_file << diagnostics_severity_name(severity) << ": ";

    diagnostics_file << message << std::endl;

    if (location.empty() || location.start.empty()) {
        return;
    }

    try {
        const auto& line = FileReader::global.get_line(location.file, location.start.line_number);
        print_expanded_line(diagnostics_file, line);

        auto width = location.width();
        if (width > 0) {
            auto position = underscore_line(line, 1, location.start.column - 1, ' ');
            underscore_line(line, location.start.column, width, '^', position);
            diagnostics_file << std::endl;
        }
    }
    catch (...) {
    }
}


size_t DiagnosticOutput::underscore_line(std::string_view line, size_t start_column, size_t width, char underline_char, size_t position) const {
    for (auto index = start_column - 1; index < std::min(start_column + width - 1, line.size()); index += 1) {
        if (line[index] == '\t') {
            do {
                diagnostics_file << underline_char;
                position += 1;
            } while (position % 8 != 0);
        }
        else {
            diagnostics_file << underline_char;
            position += 1;
        }
    }
    return position;
}

void DiagnosticOutput::print_expanded_line(std::ostream& stream, std::string_view line) const {
    size_t position = 0;
    for (auto c : line) {
        if (c == '\t') {
            do {
                stream << ' ';
                position += 1;
            } while (position % 8 != 0);
        }
        else if (c == '\n') {
            stream << c;
            position = 0;
        }
        else {
            stream << c;
            position += 1;
        }
    }
    stream << std::endl;
}

} // namespace tpau::cpp_kernal
