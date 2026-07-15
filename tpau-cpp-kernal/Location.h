#ifndef HAD_TPAU_CPP_KERNAL_LOCATION_H
#define HAD_TPAU_CPP_KERNAL_LOCATION_H

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

#include <string>

#include "Symbol.h"

namespace tpau::cpp_kernal {

/**
 * Represents a location within a file, including the file name, start position, and end position.
 *
 * Line and column numbers are 1-based, and `0` indicates that it is not specified.
 */
class Location {
  public:
    /**
     * A position within a file.
     *
     * Line and column numbers are 1-based, and `0` indicates that it is not specified.
     */
    class Position {
      public:
        /// @brief Create an empty position.
        Position() = default;

        /**
         * Create a position.
         *
         * @param line_number The line number, or `0` if the line number is not specified.
         * @param column The column number, or `0` if the column number is not specified.
         */
        Position(size_t line_number, size_t column) : line_number(line_number), column(column) {}

        /**
         * Check if two positions are equal.
         *
         * @param other The position to compare with.
         * @return `true` if the positions are equal, `false` otherwise.
         */
        bool operator==(const Position& other) const { return line_number == other.line_number && column == other.column; }

        /**
         * Check if two positions are not equal.
         *
         * @param other The position to compare with.
         * @return `true` if the positions are not equal, `false` otherwise.
         */
        bool operator!=(const Position& other) const { return !(*this == other); }

        /**
         * Check if this position is before another position.
         *
         * @param other The position to compare with.
         * @return `true` if this position is before the other position, `false` otherwise.
         */
        bool operator<(const Position& other) const;

        /**
         * Check if this position is before or equal to another position.
         *
         * @param other The position to compare with.
         * @return `true` if this position is before or equal to the other position, `false` otherwise.
         */
        bool operator<=(const Position& other) const { return !(*this > other); }

        /**
         * Check if this position is after another position.
         *
         * @param other The position to compare with.
         * @return `true` if this position is after the other position, `false` otherwise.
         */
        bool operator>(const Position& other) const;

        /**
         * Check if this position is after or equal to another position.
         *
         * @param other The position to compare with.
         * @return `true` if this position is after or equal to the other position, `false` otherwise.
         */
        bool operator>=(const Position& other) const { return !(*this < other); }

        /**
         * Convert the position to a string representation.
         *
         * @return The string representation of the position.
         */
        [[nodiscard]]
        std::string to_string() const;

        /**
         * Check if the position is empty (i.e. has no line number).
         */
        [[nodiscard]] bool empty() const { return line_number == 0; }

        operator bool() const { return !empty(); } // NOLINT(*-explicit-constructor)

        /**
         * Check if the line number is specified.
         *
         * @return `true` if the line number is specified, `false` otherwise.
         */
        [[nodiscard]] bool has_line() const { return line_number > 0; }

        /**
         * Check if the column number is specified.
         *
         * @return `true` if the column number is specified, `false` otherwise.
         */
        [[nodiscard]] bool has_column() const { return column > 0; }

        /**
         * Get the 0-based line index of the position. If the line number is not specified, `0` is returned.
         *
         * @return The 0-based line index of the position, or `0` if the line number is not specified.
         */
        size_t line_index() const { return line_number > 0 ? line_number - 1 : 0; }

        /**
         * Get the 0-based column index of the position. If the column number is not specified, `0` is returned.
         *
         * @return The 0-based column index of the position, or `0` if the column number is not specified.
         */
        size_t column_index() const { return column > 0 ? column - 1 : 0; }

        /// @brief The line number, or `0` if the line number is not specified.
        size_t line_number{0};

        /// @brief The column number, or `0` if the column number is not specified.
        size_t column{0};
    };

    /// @brief Empty location.
    Location() = default;

    /**
     * Create a location with only a file name.
     *
     * @param file_name The name of the file.
     */
    explicit Location(Symbol file_name) : file(file_name) {}

    /**
     * Create a location.
     *
     * @param file_name The name of the file.
     * @param line_number The line number, or `0` if the line number is not specified.
     * @param start_column The starting column number, or `0` if the column number is not specified.
     * @param end_column The ending column number, or `0` if the column number is not specified.
     */
    Location(Symbol file, size_t line_number, size_t start_column, size_t end_column) : file(file), start(line_number, start_column), end(line_number, end_column) { validate(); }

    /**
     * Create a location with a file name, line number, and column range.
     *
     * @param file The file symbol.
     * @param start_line_number The starting line number, or `0` if the line number is not specified.
     * @param start_column The starting column number, or `0` if the column number is not specified.
     * @param end_line_number The ending line number, or `0` if the line number is not specified.
     * @param end_column The ending column number, or `0` if the column number is not specified.
     */
    Location(Symbol file, size_t start_line_number, size_t start_column, size_t end_line_number, size_t end_column) : file(file), start(start_line_number, start_column), end(end_line_number, end_column) { validate(); }

    /**
     * Create a location that covers the range from the start of one location to the end of another location. If the two locations are in different files, the second location will be ignored.
     *
     * @param a The first location.
     * @param b The second location.
     */
    Location(const Location& a, const Location& b);

    /**
     * Extend the location to include another location. If the other location is not in the same file, it will be ignored.
     */
    void extend(const Location& other);

    /**
     * Extend the location by a number of columns.
     *
     * @param amount The number of columns to extend the location by.
     */
    void extend(size_t amount) { end.column += amount; }

    /**
     * Check if the location is within a single line.
     *
     * @return `true` if the location is within a single line, `false` otherwise.
     */
    [[nodiscard]] bool is_one_line() const { return start.line_number != 0 && start.line_number == end.line_number; }

    /**
     * Get the width of the location in columns.
     *
     * If the location is not on a single line, or no start or end column is known, `0` is returned.
     *
     * @return The width of the location in columns, or `0` if it is not well defined.
     */
    [[nodiscard]] size_t width() const;

    /**
     * Check if the location is empty (i.e. has no file).
     *
     * @return `true` if the location is empty, `false` otherwise.
     */
    [[nodiscard]] bool empty() const { return file.empty(); }

    /**
     * Check if two locations are equal.
     *
     * @param other The location to compare with.
     * @return `true` if the locations are equal, `false` otherwise.
     */
    bool operator==(const Location& other) const;

    /**
     * Check if two locations are not equal.
     */
    bool operator!=(const Location& other) const { return !(*this == other); }

    /**
     * Check if this location is before another location.
     *
     * Locations are ordered first by line number, then by column number. Locations in different files are not comparable.
     *
     * @param other The location to compare with.
     * @return `true` if this location is before the other location, `false` otherwise.
     */
    bool operator<(const Location& other) const;

    /**
     * Check if this location is after another location.
     *
     * Locations are ordered first by line number, then by column number. Locations in different files are not comparable.
     *
     * @param other The location to compare with.
     * @return `true` if this location is after the other location, `false` otherwise.
     */
    bool operator>(const Location& other) const;

    /**
     * Check if this location is before or equal to another location.
     *
     * @param other The location to compare with.
     * @return `true` if this location is before or equal to the other location, `false` otherwise.
     */
    bool operator<=(const Location& other) const { return !(*this > other); }

    /**
     * Check if this location is after or equal to another location.
     *
     * @param other The location to compare with.
     * @return `true` if this location is after or equal to the other location, `false` otherwise.
     */
    bool operator>=(const Location& other) const { return !(*this < other); }

    /**
     * Convert the location to a string representation of the start of the location.
     *
     * @return A string representation of the start of the location.
     */
    [[nodiscard]] std::string to_string() const;

    /// @brief The file.
    Symbol file;

    /// @brief The position where the location starts.
    Position start;

    /// @brief The position where the location ends.
    Position end;

  private:
    /**
     * Check that end is not before start.
     *
     * @throws Exception If location is invalid.
     */
    void validate() const;
};


/**
 * Output location to a stream.
 *
 * @param os The stream to output to.
 * @param location The location to output.
 * @return The stream after outputting the location.
 */
std::ostream& operator<<(std::ostream& os, const Location& location);

} // namespace tpau::cpp_kernal

template <> struct std::hash<tpau::cpp_kernal::Location::Position> {
    std::size_t operator()(const tpau::cpp_kernal::Location::Position& position) const noexcept { return std::hash<size_t>()(position.line_number) ^ (std::hash<size_t>()(position.column) << 1); }
};
template <> struct std::hash<tpau::cpp_kernal::Location> {
    std::size_t operator()(const tpau::cpp_kernal::Location& location) const noexcept { return std::hash<tpau::cpp_kernal::Symbol>()(location.file) ^ (std::hash<tpau::cpp_kernal::Location::Position>()(location.start) << 2) ^ (std::hash<tpau::cpp_kernal::Location::Position>()(location.end) << 4); }
};

#endif // HAD_TPAU_CPP_KERNAL_LOCATION_H
