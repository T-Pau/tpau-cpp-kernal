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

#include "Location.h"

#include "Exception.h"

namespace tpau::cpp_kernal {

Location::Location(const Location& a, const Location& b) : Location(a) { extend(b); }

void Location::extend(const Location& other) {
    if (empty()) {
        *this = other;
    }
    else if (file != other.file) {
        // Ignore location from different file.
        return;
    }

    if (other.end < start) {
        start = other.end;
    }
    else if (other.end > end) {
        end = other.end;
    }
}

std::string Location::to_string() const {
    if (!file) {
        return "";
    }
    auto s = file.str();
    if (start) {
        s += ":" + start.to_string();
    }
    return s;
}


void Location::validate() const {
    if (end < start) {
        throw std::runtime_error("Invalid location: end is before start");
    }
}


bool Location::operator==(const Location& other) const { return file == other.file && start == other.start && end == other.end; }


bool Location::operator<(const Location& other) const {
    if (file != other.file) {
        return false;
    }
    if (start != other.start) {
        return start < other.start;
    }
    return end < other.end;
}


bool Location::operator>(const Location& other) const {
    if (file != other.file) {
        return false;
    }
    if (start != other.start) {
        return start > other.start;
    }
    return end > other.end;
}


std::string Location::Position::to_string() const {
    if (line_number == 0) {
        return "";
    }
    auto s = std::to_string(line_number);
    if (column > 0) {
        s += ":" + std::to_string(column);
    }
    return s;
}

bool Location::Position::operator<(const Position& other) const {
    if (line_number != other.line_number) {
        return line_number < other.line_number;
    }
    return column < other.column;
}

bool Location::Position::operator>(const Position& other) const {
    if (line_number != other.line_number) {
        return line_number > other.line_number;
    }
    return column > other.column;
}

std::ostream& operator<<(std::ostream& os, const Location& location) {
    os << location.to_string();
    return os;
}

size_t Location::width() const {
    if (!is_one_line() || start.column == 0 || end.column == 0) {
        return 0;
    }
    else if (start.column == end.column) {
        return 1;
    }
    else {
        return end.column - start.column;
    }
}

} // namespace tpau::cpp_kernal