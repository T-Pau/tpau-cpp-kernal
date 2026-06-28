#ifndef HAD_TPAU_CPP_KERNAL_EXCEPTION_H
#define HAD_TPAU_CPP_KERNAL_EXCEPTION_H

/*
Copyright (C) Dieter Baron and Thomas Klausner

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

#include <exception>
#include <format>
#include <optional>
#include <string>

namespace tpau::cpp_kernal {

/**
 * Base class for Kernal exceptions.
 */
class Exception : public std::exception {
  public:
    Exception() = default;

    /**
     * Create an exception with the given message.
     *
     * @param message The message of the exception.
     */
    explicit Exception(std::string message) : message(std::move(message)) {}

    /**
     * Create an exception with a formatted message.
     *
     * @param format The format string for the message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> Exception(std::string format, Args&&... args) : message(std::vformat(format, std::make_format_args(args...))) {}

    /**
     * Create a new exception with additional information appended to the message.
     *
     * @param str The information to append.
     * @return The exception with the appended information.
     */
    Exception append_detail(const std::string& str);

    /**
     * Create a new exception with additional information about a system error appended to the message.
     *
     * @param code The error code to append. If not given, the current value of `errno` is used.
     * @return The exception with the appended information.
     */
    Exception append_system_error(std::optional<int> code); // default: use current errno

    /**
     * Get the message of the exception.
     *
     * @return The message of the exception.
     */
    [[nodiscard]] const char* what() const noexcept override;

  protected:
    /// The message of the exception.
    std::string message;
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_EXCEPTION_H
