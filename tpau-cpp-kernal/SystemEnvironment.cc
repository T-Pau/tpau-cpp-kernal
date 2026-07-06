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

#include "Exception.h"
#include "Util.h"


namespace tpau::cpp_kernal {

#if defined(HAVE_GET_ENVIRONMENT_VARIABLE_A) && defined(HAVE_SET_ENVIRONMENT_VARIABLE_A)
// WIndows Runtime compatible implementation using GetEnvironmentVariableA and SetEnvironmentVariableA

std::optional<std::string> SystemEnvironment::get(const std::string& name) {
    auto ret = GetEnvironmentVariableA(name.c_str(), nullptr, 0);
    if (ret == 0) {
        auto error = GetLastError();
        if (error == ERROR_ENVVAR_NOT_FOUND) {
            return {};
        }
        throw Exception("can't get length of environment variable '{}': {}", name, std::system_category().message(error));
    }
    std::string value(ret, '\0');
    auto ret2 = GetEnvironmentVariableA(name.c_str(), value.data(), ret);
    if (ret2 == 0) {
        auto error = GetLastError();
        throw Exception("can't get environment variable '{}': {}", name, std::system_category().message(error));
    }
    value.resize(ret2);
    return value;
}

bool SystemEnvironment::is_set(const std::string& name) {
    auto ret = GetEnvironmentVariableA(name.c_str(), nullptr, 0);
    if (ret == 0) {
        auto error = GetLastError();
        if (error == ERROR_ENVVAR_NOT_FOUND) {
            return false;
        }
        throw Exception("can't get length of environment variable '{}': {}", name, std::system_category().message(error));
    }
    return true;
}

void SystemEnvironment::set(const std::string& name, const std::string& value, bool overwrite) {
    if (overwrite || !is_set(name)) {
        if (!SetEnvironmentVariableA(name.c_str(), value.c_str())) {
            auto error = GetLastError();
            throw Exception("can't set environment variable '{}': {}", name, std::system_category().message(error));
        }
    }
}

void SystemEnvironment::unset(const std::string& name) {
    if (!SetEnvironmentVariableA(name.c_str(), nullptr)) {
        auto error = GetLastError();
        throw Exception("can't unset environment variable '{}': {}", name, std::system_category().message(error));
    }
}


#elif defined(HAVE_GETENV_S) && defined(HAVE__PUTENV_S)
// "Save" implementation using getenv_s and _putenv_s

std::optional<std::string> SystemEnvironment::get(const std::string& name) {
    size_t required_size{}, required_size2{};
    auto ret = getenv_s(&required_size, nullptr, 0, name.c_str());
    if (ret != 0 && ret != -1 /* not found */) {
        throw Exception("can't get length of environment variable '{}': {}", name, strerror_string(-ret));
    }
    if (required_size == 0) {
        return {};
    }
    std::string value(required_size, '\0');
    ret = getenv_s(&required_size2, value.data(), required_size, name.c_str());
    if (ret != 0 && ret != -1 /* not found */) {
        throw Exception("can't get environment variable '{}': {}", name, strerror_string(-ret));
    }
    value.resize(required_size - 1); // Remove the NUL terminator
    return value;
}

void SystemEnvironment::set(const std::string& name, const std::string& value, bool overwrite) {
    if (overwritde || is_set(name)) {
        auto ret = _putenv_s(name.c_str(), value.c_str());
        if (ret != 0) {
            throw Exception("can't set environment variable '{}': {}", name, strerror_string(-ret));
        }
    }
}

void SystemEnvironment::unset(const std::string& name) {
    auto ret = _putenv_s(name.c_str(), "");
    if (ret != 0) {
        throw Exception("can't unset environment variable '{}': {}", name, strerror_string(-ret));
    }
}

bool SystemEnvironment::is_set(const std::string& name) {
    size_t required_size{};
    auto ret = getenv_s(&required_size, nullptr, 0, name.c_str());
    if (ret != 0 && ret != -1 /* not found */) {
        throw Exception("can't get length of environment variable '{}': {}", name, strerror_string(-ret));
    }
    return required_size > 0;
}

#else
// POSIX compatible implementation using getenv, setenv, and unsetenv

std::optional<std::string> SystemEnvironment::get(const std::string& name) {
    auto value = getenv(name.c_str());
    if (value == nullptr) {
        return {};
    }
    return std::string(value);
}

void SystemEnvironment::set(const std::string& name, const std::string& value, bool overwrite) {
    if (setenv(name.c_str(), value.c_str(), overwrite ? 1 : 0) != 0) {
        throw Exception("can't set environment variable '{}': {}", name, strerror_string());
    }
}

void SystemEnvironment::unset(const std::string& name) {
    if (unsetenv(name.c_str()) != 0) {
        throw Exception("can't unset environment variable '{}': {}", name, strerror_string());
    }
}

bool SystemEnvironment::is_set(const std::string& name) {
    return getenv(name.c_str()) != nullptr;
}

#endif

} // namespace tpau::cpp_kernal
