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

    /**
     * Output a notice with stream interface.
     *
     * @param category The category of the notice.
     * @param location The location of the notice.
     * @return A stream to output the notice message to.
     */
    Stream notice(Symbol category = {}, const Location& location = {}) { return output(category, NOTICE, location); }

    /**
     * Output a formatted notice.
     *
     * @param category The category of the notice.
     * @param location The location of the notice.
     * @param format The format string for the notice message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void notice(Symbol category, const Location& location, std::string_view format, Args&&... args) { output(category, NOTICE, location, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a formatted notice with default category.
     *
     * @param location The location of the notice.
     * @param format The format string for the notice message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void notice(const Location& location, std::string_view format, Args&&... args) { output(Symbol{}, NOTICE, location, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a formatted notice without location.
     *
     * @param category The category of the notice.
     * @param format The format string for the notice message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void notice(Symbol category, std::string_view format, Args&&... args) { output(category, NOTICE, Location{}, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a formatted notice with default category and without location.
     *
     * @param format The format string for the notice message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void notice(std::string_view format, Args&&... args) { output(Symbol{}, NOTICE, Location{}, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a notice message.
     *
     * @param category The category of the notice.
     * @param location The location of the notice.
     * @param message The message of the notice.
     */
    void notice(Symbol category, const Location& location, std::string_view message) { output(category, NOTICE, location, message); }

    /**
     * Output a notice message with default category.
     *
     * @param location The location of the notice.
     * @param message The message of the notice.
     */
    void notice(const Location& location, std::string_view message) { output(Symbol{}, NOTICE, location, message); }

    /**
     * Output a notice message without location.
     *
     * @param message The message of the notice.
     */
    void notice(Symbol category, std::string_view message) { output(category, NOTICE, Location{}, message); }

    /**
     * Output a notice message with default category and without location.
     *
     * @param message The message of the notice.
     */
    void notice(std::string_view message) { output(Symbol{}, NOTICE, Location{}, message); }

    /**
     * Output a warning with stream interface.
     *
     * @param category The category of the warning.
     * @param location The location of the warning.
     * @return A stream to output the warning message to.
     */
    Stream warning(Symbol category = {}, const Location& location = {}) { return output(category, WARNING, location); }

    /**
     * Output a formatted warning.
     *
     * @param category The category of the warning.
     * @param location The location of the warning.
     * @param format The format string for the warning message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void warning(Symbol category, const Location& location, std::string_view format, Args&&... args) { output(category, WARNING, location, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a formatted warning with default category.
     *
     * @param location The location of the warning.
     * @param format The format string for the warning message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void warning(const Location& location, std::string_view format, Args&&... args) { output(Symbol{}, WARNING, location, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a formatted warning without location.
     *
     * @param category The category of the warning.
     * @param format The format string for the warning message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void warning(Symbol category, std::string_view format, Args&&... args) { output(category, WARNING, Location{}, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a formatted warning with default category and without location.
     *
     * @param format The format string for the warning message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void warning(std::string_view format, Args&&... args) { output(Symbol{}, WARNING, Location{}, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a warning.
     *
     * @param category The category of the warning.
     * @param location The location of the warning.
     * @param message The message of the warning.
     */
    void warning(Symbol category, const Location& location, std::string_view message) { output(category, WARNING, location, message); }

    /**
     * Output a warning with default category.
     *
     * @param location The location of the warning.
     * @param message The message of the warning.
     */
    void warning(const Location& location, std::string_view message) { output(Symbol{}, WARNING, location, message); }

    /**
     * Output a warning without location.
     *
     * @param category The category of the warning.
     * @param message The message of the warning.
     */
    void warning(Symbol category, std::string_view message) { output(category, WARNING, Location{}, message); }

    /**
     * Output a warning with default category and without location.
     *
     * @param message The message of the warning.
     */
    void warning(std::string_view message) { output(Symbol{}, WARNING, Location{}, message); }

    /**
     * Output an error message with stream interface.
     *
     * @param category The category of the error.
     * @param location The location of the error.
     * @return A stream to output the error message to.
     */
    Stream error(Symbol category = {}, const Location& location = {}) { return output(category, ERROR, location); }

    /**
     * Output a formatted error message.
     *
     * @param category The category of the error.
     * @param location The location of the error.
     * @param format The format string for the error message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void error(Symbol category, const Location& location, std::string_view format, Args&&... args) { output(category, ERROR, location, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a formatted error message with default category.
     *
     * @param location The location of the error.
     * @param format The format string for the error message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void error(const Location& location, std::string_view format, Args&&... args) { output(Symbol{}, ERROR, location, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a formatted error message without location.
     *
     * @param category The category of the error.
     * @param format The format string for the error message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void error(Symbol category, std::string_view format, Args&&... args) { output(category, ERROR, Location{}, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a formatted error message with default category and without location.
     *
     * @param format The format string for the error message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void error(std::string_view format, Args&&... args) { output(Symbol{}, ERROR, Location{}, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output an error message.
     *
     * @param category The category of the error.
     * @param location The location of the error.
     * @param message The message of the error.
     */
    void error(Symbol category, const Location& location, std::string_view message) { output(category, ERROR, location, message); }

    /**
     * Output an error message with default category.
     *
     * @param location The location of the error.
     * @param message The message of the error.
     */
    void error(const Location& location, std::string_view message) { output(Symbol{}, ERROR, location, message); }

    /**
     * Output an error message without location.
     *
     * @param category The category of the error.
     * @param message The message of the error.
     */
    void error(Symbol category, std::string_view message) { output(category, ERROR, Location{}, message); }

    /**
     * Output an error message with default category and without location.
     *
     * @param message The message of the error.
     */
    void error(std::string_view message) { output(Symbol{}, ERROR, Location{}, message); }

    /**
     * Output a message with stream interface.
     *
     * @param category The category of the message.
     * @param severity The severity of the message.
     * @param location The location of the message.
     * @return A stream to output the message to.
     */
    Stream output(Symbol category, Severity severity, const Location& location);

    /**
     * Output a message with formatting.
     *
     * @param category The category of the message.
     * @param severity The severity of the message.
     * @param location The location of the message.
     * @param format The format string for the message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void output(Symbol category, Severity severity, const Location& location, std::string_view format, Args&&... args) { output(category, severity, location, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a message with default category and formatting.
     *
     * @param severity The severity of the message.
     * @param location The location of the message.
     * @param format The format string for the message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void output(Severity severity, const Location& location, std::string_view format, Args&&... args) { output(Symbol{}, severity, location, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a message without location and with formatting.
     *
     * @param category The category of the message.
     * @param severity The severity of the message.
     * @param format The format string for the message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void output(Symbol category, Severity severity, std::string_view format, Args&&... args) { output(category, severity, Location{}, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a message with default category, without location and with formatting.
     *
     * @param severity The severity of the message.
     * @param format The format string for the message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void output(Severity severity, std::string_view format, Args&&... args) { output(Symbol{}, severity, Location{}, std::vformat(format, std::make_format_args(std::forward<Args>(args)...))); }

    /**
     * Output a message with default category.
     *
     * @param severity The severity of the message.
     * @param location The location of the message.
     * @param message The message to output.
     */
    void output(Severity severity, const Location& location, std::string_view message) { output(Symbol{}, severity, location, message); }

    /**
     * Output a message without location.
     *
     * @param category The category of the message.
     * @param severity The severity of the message.
     * @param message The message to output.
     */
    void output(Symbol category, Severity severity, const Location& location, std::string_view message);

    /**
     * Output a message with default category and without location.
     *
     * @param severity The severity of the message.
     * @param message The message to output.
     */
    void output(Severity severity, std::string_view message) { output(Symbol{}, severity, Location{}, message); }

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
    void underscore_line(std::string_view line, size_t start_column, size_t width, char underline_char) const;

    static const char* diagnostics_severity_name(Severity severity);

    std::unordered_set<Symbol> ignored_categories;
    Severity fail_serverity{ERROR};
    bool fail_flag{false};
    std::ostream& diagnostics_file{std::cerr};
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_DIAGNOSTIC_OUTPUT_H
