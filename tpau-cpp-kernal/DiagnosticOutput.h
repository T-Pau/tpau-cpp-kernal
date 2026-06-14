#ifndef HAD_TPAU_CPP_KERNAL_DIAGNOSTIC_OUTPUT_H
#define HAD_TPAU_CPP_KERNAL_DIAGNOSTIC_OUTPUT_H

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

#include <iostream>

#include "Exception.h"
#include "Location.h"

#include <unordered_set>

namespace tpau::cpp_kernal {

class DiagnosticOutput {
  public:
    enum Severity { NOTICE, WARNING, ERROR };

    class Stream : public std::ostream {
      private:
        class Buffer : public std::streambuf {
          public:
            explicit Buffer(std::ostream& stream, bool output) : stream(stream), output(output) {}

            int_type overflow(int_type ch) override {
                if (output && ch != EOF) {
                    stream.put(static_cast<char>(ch));
                }
                return ch;
            }

            std::streamsize xsputn(const char_type* s, std::streamsize n) override {
                if (output && n > 0) {
                    stream.write(s, n);
                }
                return n;
            }

            int sync() override {
                if (output) {
                    stream.flush();
                }
                return 1; // TODO: correct?
            }

            std::ostream& stream;
            bool output;
        };

      public:
        Stream(DiagnosticOutput& diagnostic_output, const Location& location, std::ostream& stream, bool output) : std::ostream(&buffer), diagnostic_output(diagnostic_output), buffer(stream, output), location(location), output(output) {}
        ~Stream() override;

        const Location& location;
        bool output;
        Buffer buffer;
        DiagnosticOutput& diagnostic_output;
    };

    void register_category(Symbol category, Severity severity);

    Stream notice(Symbol category, const Location& location) { return output(category, NOTICE, location); }
    void notice(Symbol category, const Location& location, const std::string& message) { output(category, NOTICE, location, message); }
    Stream warning(Symbol category, const Location& location) { return output(category, WARNING, location); }
    void warning(Symbol category, const Location& location, const std::string& message) { output(category, WARNING, location, message); }
    Stream error(Symbol category, const Location& location) { return output(category, ERROR, location); }
    void error(Symbol category, const Location& location, const std::string& message) { output(category, ERROR, location, message); }
    Stream output(Symbol category, Severity severity, const Location& location);
    void output(Symbol category, Severity severity, const Location& location, const std::string& message);

    [[nodiscard]] bool failed() const { return fail_flag; }

    bool verbose_error_messages{false};

    static DiagnosticOutput global;

  private:
    bool begin_message(Symbol category, Severity severity, const Location& location);
    void end_message(const Location& location) const;

    /**
     * Output a character under part of a line of source code.
     *
     * It assumes that the line of source code has already been output, and the cursor is at start_column of the next line.
     *
     * @param line The line of source code to output.
     * @param start_column The column to start at.
     * @param width The number of columns to underline.
     * @param underline_char The character to use.
     */
    void underscore_line(const std::string& line, size_t start_column, size_t width, char underline_char) const;

    static const char* diagnostics_severity_name(Severity severity);

    std::unordered_set<Symbol> ignored_categories;
    Severity fail_serverity{ERROR};
    bool fail_flag{false};
    std::ostream& diagnostics_file{std::cerr};
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_DIAGNOSTIC_OUTPUT_H
