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

#include "SystemEnvironment.h"

#include "config.h"

#if defined(HAVE_SET_ENVIRONMENT_VARIABLE_A)
#include <windows.h>
#endif
#include <cstdlib>


namespace tpau::cpp_kernal {

std::optional<std::string> SystemEnvironment::get(std::string_view name) {
#if defined(HAVE_GETENV_S)
    size_t required_size{};
    getenv_s(&required_size, nullptr, 0, name.data());
    if (required_size == 0) {
        return {};
    }
    std::string value(required_size, '\0');
    getenv_s(&required_size, value.data(), required_size, name.data());
    value.resize(required_size - 1); // Remove the NUL terminator
    return value;
#else
    auto value = getenv(name.data());
    if (value == nullptr) {
        return {};
    }
    return std::string(value);
#endif
}

void SystemEnvironment::set(std::string_view name, std::string_view value, bool overwrite) {
#if defined(HAVE_SET_ENVIRONMENT_VARIABLE_A)
    if (overwrite || !is_set(name)) {
        SetEnvironmentVariableA(name.data(), value.data());
    }
#else
    setenv(name.data(), value.data(), overwrite ? 1 : 0);
#endif
}

void SystemEnvironment::unset(std::string_view name) {
#if defined(HAVE_SET_ENVIRONMENT_VARIABLE_A)
    SetEnvironmentVariableA(name.data(), nullptr);
#else
    unsetenv(name.data());
#endif
}

bool SystemEnvironment::is_set(std::string_view name) {
#if defined(HAVE_GETENV_S)
    size_t required_size{};
    getenv_s(&required_size, nullptr, 0, name.data());
    return required_size > 0;
#else
    return getenv(name.data()) != nullptr;
#endif
}

} // namespace tpau::cpp_kernal
