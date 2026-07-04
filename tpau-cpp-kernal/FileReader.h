#ifndef HAD_TPAU_CPP_KERNAL_FILE_READER_H
#define HAD_TPAU_CPP_KERNAL_FILE_READER_H

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
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Symbol.h"

namespace tpau::cpp_kernal {

/**
 * Provides an interface for reading files, with support for caching and binary files.
 */
class FileReader {
  public:
    /**
     * Get the contents of a text file.
     *
     * @param file_name The name of the file to read.
     * @param optional If `true`, return an empty file if the file does not exist.
     * @return The contents of the file as a vector of lines.
     * @throws Exception If the file does not exist and `optional` is `false`.
     */
    const std::vector<std::string>& read(Symbol file_name, bool optional = false);

    /**
     * Get the contents of the standard input.
     *
     * @return The contents of the standard input as a vector of lines.
     */
    const std::vector<std::string>& read_stdin();

    /**
     * Get the contents of a binary file.
     *
     * @param file_name The name of the file to read.
     * @param optional If `true`, return an empty string if the file does not exist.
     * @return The contents of the file as a string.
     * @throws Exception If the file does not exist and `optional` is `false`.
     */
    [[nodiscard]] std::string read_binary(Symbol file_name, bool optional = false);

    /**
     * Get a specific line from a file.
     *
     * @param file The name of the file.
     * @param line_number The line number to retrieve (0-based).
     * @return The requested line.
     * @throws Exception If the file has not been read or the line does not exist.
     */
    [[nodiscard]] const std::string& get_line(Symbol file, size_t line_number) const;

    /**
     * Get the names of all files that have been read.
     *
     * @return A vector of file names.
     */
    [[nodiscard]] std::vector<Symbol> file_names() const;

    /// @brief The global file reader instance.
    static FileReader global;

  private:
    std::vector<std::string> get_lines(std::istream& stream);

    /// @brief An empty file to return when an optional file is not found.
    static std::vector<std::string> empty_file;

    /// @brief The set of binary files that have been read.
    std::unordered_set<Symbol> binary_files;

    /// @brief The map of text file names to their contents.
    std::unordered_map<Symbol, std::vector<std::string>> files;

    /// @brief A flag indicating whether the standard input has been read.
    bool stdin_read{false};

    /// @brief The contents of the standard input.
    std::vector<std::string> stdin_lines;
};

} // namespace tpau::cpp_kernal


#endif // HAD_TPAU_CPP_KERNAL_FILE_READER_H
