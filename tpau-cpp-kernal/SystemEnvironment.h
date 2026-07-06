#ifndef HAD_TPAU_CPP_KERNAL_SYSTEM_ENVIRONMENT_H
#define HAD_TPAU_CPP_KERNAL_SYSTEM_ENVIRONMENT_H

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
#include <optional>

namespace tpau::cpp_kernal {

/**
 * Access to system environment variables.
 *
 * Since C++ does not provide a standard way to access environment variables, this class provides a platform-independent interface for getting, setting, and unsetting environment variables.
 * 
 * On Windows it uses `GetEnvironmentVariable`, `SetEnvironmentVariable`, since they are available in all build environments. However, variables changed this way won't reflect when queried using `std::getenv`.
 *
 * On POSIX systems it uses `getenv`, `setenv`, and `unsetenv`.
 */
class SystemEnvironment {
  public:
    /**
     * Get the value of an environment variable.
     *
     * @param name The name of the environment variable.
     * @return The value of the environment variable, or {} if it is not set.
     */
    [[nodiscard]] static std::optional<std::string> get(const std::string& name);
                                                                   
    /**
     * Set the value of an environment variable.
     *
     * @param name The name of the environment variable.
     * @param value The value to set the environment variable to.
     * @param overwrite Whether to overwrite the environment variable if it is already set.
     * @throws Exception If the environment variable could not be set.
     */
    static void set(const std::string& name, const std::string& value, bool overwrite = true);

    /**
     * Unset an environment variable.
     *
     * @param name The name of the environment variable.
     * @throws Exception If the environment variable could not be unset.
     */
    static void unset(const std::string& name);

    /**
     * Check if an environment variable is set.
     *
     * @param name The name of the environment variable.
     * @return `true` if the environment variable is set, `false` otherwise.
     */
    [[nodiscard]] static bool is_set(const std::string& name);
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_SYSTEM_ENVIRONMENT_H