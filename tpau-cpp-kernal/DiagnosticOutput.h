#ifndef HAD_TPAU_CPP_KERNAL_DIAGNOSTIC_OUTPUT_H
#define HAD_TPAU_CPP_KERNAL_DIAGNOSTIC_OUTPUT_H

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

#include <iostream>

#include "Location.h"
#include "LocationException.h"

#include <unordered_set>

namespace tpau::cpp_kernal {

class DiagnosticOutput {
  public:
    enum Severity { NOTE, WARNING, ERROR };

    /**
     * Create a diagnostic output.
     *
     * @param diagnostics_file The file to output diagnostics to.
     */
    explicit DiagnosticOutput(std::ostream& diagnostics_file = std::cerr) : diagnostics_file(diagnostics_file) {}

    /**
     * Register a category with a severity.
     *
     * @param category The category to register.
     * @param severity The minimum severity for messages in the category to be printed.
     */
    void register_category(Symbol category, Severity severity);

    /**
     * Output a note.
     *
     * @param category The category of the note.
     * @param location The location of the note.
     * @param message The message of the note.
     */
    void note(Symbol category, const Location& location, std::string_view message) { output(category, NOTE, location, message); }

    /**
     * Output a note with default category.
     *
     * @param location The location of the note.
     * @param message The message of the note.
     */
    void note(const Location& location, std::string_view message) { note(Symbol{}, location, message); }

    /**
     * Output a note without location.
     *
     * @param category The category of the note.
     * @param message The message of the note.
     */
    void note(Symbol category, std::string_view message) { output(category, NOTE, Location{}, message); }

    /**
     * Output a note with default category and without location.
     *
     * @param message The message of the note.
     */
    void note(std::string_view message) { note(Symbol{}, Location{}, message); }

    /**
     * Output a formatted note.
     *
     * @param category The category of the note.
     * @param location The location of the note.
     * @param format The format string for the note message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void note(Symbol category, const Location& location, std::format_string<Args...> format, Args&&... args) { note(category, location, std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a formatted note with default category.
     *
     * @param location The location of the note.
     * @param format The format string for the note message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void note(const Location& location, std::format_string<Args...> format, Args&&... args) { note(location, std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a formatted note without location.
     *
     * @param category The category of the note.
     * @param format The format string for the note message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void note(Symbol category, std::format_string<Args...> format, Args&&... args) { note(category, std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a formatted note with default category and without location.
     *
     * @param format The format string for the note message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void note(std::format_string<Args...> format, Args&&... args) { note(std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a note from an exception.
     *
     * @param category The category of the note.
     * @param location The location of the note.
     * @param exception The exception.
     */
    void note(Symbol category, const Location& location, const Exception& exception) { note(category, Location{}, exception.what()); }

    /**
     * Output a note from an exception with default category.
     *
     * @param location The location of the note.
     * @param exception The exception.
     */
    void note(const Location& location, const Exception& exception) { note(location, exception.what()); }

    /**
     * Output a note from an exception without location.
     *
     * @param category The category of the note.
     * @param exception The exception.
     */
    void note(Symbol category, const Exception& exception) { note(category, exception.what()); }

    /**
     * Output a note from an exception with default category and without location.
     *
     * @param exception The exception.
     */
    void note(const Exception& exception) { note(exception.what()); }

    /**
     * Output a note from a location exception.
     *
     * @param category The category of the note.
     * @param exception The location exception.
     */
    void note(Symbol category, const LocationException& exception) { note(category, exception.location, exception.what()); }

    /**
     * Output a note from a location exception with default category.
     *
     * @param exception The location exception.
     */
    void note(const LocationException& exception) { note(exception.location, exception.what()); }


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
    void warning(const Location& location, std::string_view message) { warning(Symbol{}, location, message); }

    /**
     * Output a warning without location.
     *
     * @param category The category of the warning.
     * @param message The message of the warning.
     */
    void warning(Symbol category, std::string_view message) { warning(category, Location{}, message); }

    /**
     * Output a warning with default category and without location.
     *
     * @param message The message of the warning.
     */
    void warning(std::string_view message) { warning(Symbol{}, Location{}, message); }

    /**
     * Output a formatted warning.
     *
     * @param category The category of the warning.
     * @param location The location of the warning.
     * @param format The format string for the warning message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void warning(Symbol category, const Location& location, std::format_string<Args...> format, Args&&... args) { warning(category, location, std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a formatted warning with default category.
     *
     * @param location The location of the warning.
     * @param format The format string for the warning message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void warning(const Location& location, std::format_string<Args...> format, Args&&... args) { warning(location, std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a formatted warning without location.
     *
     * @param category The category of the warning.
     * @param format The format string for the warning message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void warning(Symbol category, std::format_string<Args...> format, Args&&... args) { warning(category, std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a formatted warning with default category and without location.
     *
     * @param format The format string for the warning message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void warning(std::format_string<Args...> format, Args&&... args) { warning(std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a warning from an exception.
     *
     * @param category The category of the warning.
     * @param location The location of the warning.
     * @param exception The exception.
     */
    void warning(Symbol category, const Location& location, const Exception& exception) { warning(category, Location{}, exception.what()); }

    /**
     * Output a warning from an exception with default category.
     *
     * @param location The location of the warning.
     * @param exception The exception.
     */
    void warning(const Location& location, const Exception& exception) { warning(location, exception.what()); }

    /**
     * Output a warning from an exception without location.
     *
     * @param category The category of the warning.
     * @param exception The exception.
     */
    void warning(Symbol category, const Exception& exception) { warning(category, exception.what()); }

    /**
     * Output a warning from an exception with default category and without location.
     *
     * @param exception The exception.
     */
    void warning(const Exception& exception) { warning(exception.what()); }

    /**
     * Output a warning from a location exception.
     *
     * @param category The category of the warning.
     * @param exception The location exception.
     */
    void warning(Symbol category, const LocationException& exception) { warning(category, exception.location, exception.what()); }

    /**
     * Output a warning from a location exception with default category.
     *
     * @param exception The location exception.
     */
    void warning(const LocationException& exception) { warning(exception.location, exception.what()); }


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
    void error(const Location& location, std::string_view message) { error(Symbol{}, location, message); }

    /**
     * Output an error message without location.
     *
     * @param category The category of the error.
     * @param message The message of the error.
     */
    void error(Symbol category, std::string_view message) { error(category, Location{}, message); }

    /**
     * Output an error message with default category and without location.
     *
     * @param message The message of the error.
     */
    void error(std::string_view message) { error(Symbol{}, Location{}, message); }

    /**
     * Output a formatted error message.
     *
     * @param category The category of the error.
     * @param location The location of the error.
     * @param format The format string for the error message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void error(Symbol category, const Location& location, std::format_string<Args...> format, Args&&... args) { error(category, location, std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a formatted error message with default category.
     *
     * @param location The location of the error.
     * @param format The format string for the error message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void error(const Location& location, std::format_string<Args...> format, Args&&... args) { error(location, std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a formatted error message without location.
     *
     * @param category The category of the error.
     * @param format The format string for the error message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void error(Symbol category, std::format_string<Args...> format, Args&&... args) { error(category, std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a formatted error message with default category and without location.
     *
     * @param format The format string for the error message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void error(std::format_string<Args...> format, Args&&... args) { error(std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output an error from an exception.
     *
     * @param category The category of the error.
     * @param location The location of the error.
     * @param exception The exception.
     */
    void error(Symbol category, const Location& location, const Exception& exception) { error(category, Location{}, exception.what()); }

    /**
     * Output an error from an exception with default category.
     *
     * @param location The location of the error.
     * @param exception The exception.
     */
    void error(const Location& location, const Exception& exception) { error(location, exception.what()); }

    /**
     * Output an error from an exception without location.
     *
     * @param category The category of the error.
     * @param exception The exception.
     */
    void error(Symbol category, const Exception& exception) { error(category, exception.what()); }

    /**
     * Output an error from an exception with default category and without location.
     *
     * @param exception The exception.
     */
    void error(const Exception& exception) { error(exception.what()); }

    /**
     * Output an error from a location exception.
     *
     * @param category The category of the error.
     * @param exception The location exception.
     */
    void error(Symbol category, const LocationException& exception) { error(category, exception.location, exception.what()); }

    /**
     * Output an error from a location exception with default category.
     *
     * @param exception The location exception.
     */
    void error(const LocationException& exception) { error(exception.location, exception.what()); }


    /**
     * Output a message.
     *
     * @param category The category of the message.
     * @param severity The severity of the message.
     * @param location The location of the message.
     * @param message The message to output.
     */
    void output(Symbol category, Severity severity, const Location& location, std::string_view message);

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
    void output(Symbol category, Severity severity, std::string_view message) { output(category, severity, Location{}, message); }

    /**
     * Output a message with default category and without location.
     *
     * @param severity The severity of the message.
     * @param message The message to output.
     */
    void output(Severity severity, std::string_view message) { output(Symbol{}, severity, Location{}, message); }

    /**
     * Output a formatted message.
     *
     * @param category The category of the message.
     * @param severity The severity of the message.
     * @param location The location of the message.
     * @param format The format string for the message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void output(Symbol category, Severity severity, const Location& location, std::format_string<Args...> format, Args&&... args) { output(category, severity, location, std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a formatted message with default category.
     *
     * @param severity The severity of the message.
     * @param location The location of the message.
     * @param format The format string for the message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void output(Severity severity, const Location& location, std::format_string<Args...> format, Args&&... args) { output(Symbol{}, severity, location, std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a formatted message without location.
     *
     * @param category The category of the message.
     * @param severity The severity of the message.
     * @param format The format string for the message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void output(Symbol category, Severity severity, std::format_string<Args...> format, Args&&... args) { output(category, severity, Location{}, std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a formatted message with default category, without location.
     *
     * @param severity The severity of the message.
     * @param format The format string for the message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> void output(Severity severity, std::format_string<Args...> format, Args&&... args) { output(Symbol{}, severity, Location{}, std::format(format, std::forward<Args>(args)...)); }

    /**
     * Output a message from an exception.
     *
     * @param category The category of the message.
     * @param severity The severity of the message.
     * @param location The location of the message.
     * @param exception The exception.
     */
    void output(Symbol category, Severity severity, const Location& location, const Exception& exception) { output(category, severity, location, exception.what()); }

    /**
     * Output a message from an exception with default category.
     *
     * @param severity The severity of the message.
     * @param location The location of the message.
     * @param exception The exception.
     */
    void output(Severity severity, const Location& location, const Exception& exception) { output(Symbol{}, severity, location, exception.what()); }

    /**
     * Output a message from an exception without location.
     *
     * @param category The category of the message.
     * @param severity The severity of the message.
     * @param exception The exception.
     */
    void output(Symbol category, Severity severity, const Exception& exception) { output(category, severity, Location{}, exception.what()); }

    /**
     * Output a message from an exception with default category and without location.
     *
     * @param severity The severity of the message.
     * @param exception The exception.
     */
    void output(Severity severity, const Exception& exception) { output(Symbol{}, severity, Location{}, exception.what()); }

    /**
     * Output a message from a location exception.
     *
     * @param category The category of the message.
     * @param severity The severity of the message.
     * @param exception The location exception.
     */
    void output(Symbol category, Severity severity, const LocationException& exception) { output(category, severity, exception.location, exception.what()); }

    /**
     * Output a message from a location exception with default category.
     *
     * @param severity The severity of the message.
     * @param exception The location exception.
     */
    void output(Severity severity, const LocationException& exception) { output(Symbol{}, severity, exception.location, exception.what()); }


    /**
     * Check if the program has been marked as failed.
     *
     * This can either been done explicitly by calling `mark_failed()`, or implicitly by outputting a message with a severity equal to or higher than the fail severity, which defaults to error.
     */
    [[nodiscard]] bool failed() const { return fail_flag; }

    /**
     * Mark the program as failed.
     */
    void mark_failed() { fail_flag = true; }

    bool verbose_error_messages{false};

    static DiagnosticOutput global;

  private:
    /**
     * Output a line of source code, expanding tabs to spaces.
     *
     * @param stream The stream to output to.
     * @param line The line of source code to output.
     */
    void print_expanded_line(std::ostream& stream, std::string_view line) const;

    /**
     * Output a character under part of a line of source code.
     *
     * This function handles tabs in the source code line, assuming a tab width of 8 characters.
     *
     * It assumes that the line of source code has already been output.
     *
     * @param line The line of source code to underline.
     * @param start_column The position to start at; the start of the line is 1.
     * @param width The number of columns to underline.
     * @param underline_char The character to use.
     * @param position The position in the line the cursor is at; the beginning of the line is 0.
     * @return The position of the cursor after outputting the underline characters; the beginning of the line is 0.
     */
    size_t underscore_line(std::string_view line, size_t start_column, size_t width, char underline_char, size_t position = 0) const;

    static const char* diagnostics_severity_name(Severity severity);

    std::unordered_set<Symbol> ignored_categories;
    Severity fail_severity{ERROR};
    bool fail_flag{false};
    std::ostream& diagnostics_file{std::cerr};
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_DIAGNOSTIC_OUTPUT_H
