#ifndef HAD_TPAU_CPP_KERNAL_FILE_SOURCE_H
#define HAD_TPAU_CPP_KERNAL_FILE_SOURCE_H

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

#include <cstdio>

#include "Location.h"
#include "Symbol.h"

namespace tpau::cpp_kernal {

/**
 * Represents a source of characters from a file, with support for lookahead and seeking.
 */
class FileSource {
  public:
    FileSource(Symbol filename);

    /**
     * Get the next character from the file. Returns `EOF` if the end of the file is reached.
     *
     * @return The next character from the file, or `EOF` if the end of the file is reached.
     */
    int get();

    /**
     * Unget the last character read from the file. The next call to `get()` will return the same character again.
     *
     * @throws Exception If at the beginning of the file.
     */
    void unget();

    /**
     * Get the name of the file.
     *
     * @return The name of the file.
     */
    [[nodiscard]] Symbol filename() const { return filename_; }

    /**
     * Get the current location in the file.
     *
     * @return The current location in the file.
     */
    [[nodiscard]] Location location() const { return {filename(), line_index + 1, column_index + 1, column_index + 1}; }

    /**
     * Expand a location within the file to include the current location.
     *
     * If the location is not within the file, it will be left unchanged.
     *
     * @param location The location to expand.
     */
    void expand_location(Location& location) const;

    /**
     * Reset the current position to a new location.
     *
     * @param new_location The new location to reset to.
     * @throws Exception If the new location is not within the file.
     */
    void reset_to(const Location& new_location);

  private:
    /// @brief The name of the file.
    Symbol filename_;

    /// @brief The lines of the file.
    const std::vector<std::string>& lines;

    /// @brief The current line number (0-based).
    size_t line_index{0};

    /// @brief The current column number (0-based).
    size_t column_index{0};
};

} // namespace tpau::cpp_kernal


#endif // HAD_TPAU_CPP_KERNAL_FILE_SOURCE_H
