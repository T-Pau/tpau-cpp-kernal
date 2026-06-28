#ifndef HAD_TPAU_CPP_KERNAL_LOCATION_EXCEPTION_H
#define HAD_TPAU_CPP_KERNAL_LOCATION_EXCEPTION_H

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
#include <optional>
#include <string>

#include "Exception.h"
#include "Location.h"

namespace tpau::cpp_kernal {

/**
 * Kernal exceptions with location.
 */
class LocationException : public Exception {
  public:
    LocationException() = default;

    /**
     * Create an exception with the given message.
     *
     * @param message The message of the exception.
     */
    explicit LocationException(Location location, std::string message) : Exception(std::move(message)), location(std::move(location)) {}

    /**
     * Create an exception with a formatted message.
     *
     * @param format The format string for the message.
     * @param args The arguments for the format string.
     */
    template <typename... Args> LocationException(Location location, std::string format, Args&&... args) : Exception(std::vformat(format, std::make_format_args(args...))), location(std::move(location)) {}

  protected:
    /// The location of the exception.
    Location location;
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_LOCATION_EXCEPTION_H
