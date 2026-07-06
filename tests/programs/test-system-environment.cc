#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include "SystemEnvironment.h"

TEST_CASE("SystemEnvironment: get set variable", "[system_environment]") {
    auto value = tpau::cpp_kernal::SystemEnvironment::get("PATH");
    REQUIRE(value.has_value());
    REQUIRE(!value->empty());
}

TEST_CASE("SystemEnvironment: get unset variable", "[system_environment]") {
    auto value = tpau::cpp_kernal::SystemEnvironment::get("UNSET_VARIABLE");
    REQUIRE(!value.has_value());
}

TEST_CASE("SystemEnvironment: set and get variable", "[system_environment]") {
    tpau::cpp_kernal::SystemEnvironment::set("TEST_VARIABLE", "value");
    auto value = tpau::cpp_kernal::SystemEnvironment::get("TEST_VARIABLE");
    REQUIRE(value.has_value());
    REQUIRE(*value == "value");
    tpau::cpp_kernal::SystemEnvironment::unset("TEST_VARIABLE");
}

TEST_CASE("SystemEnvironment: not overwriting variable", "[system_environment]") {
    tpau::cpp_kernal::SystemEnvironment::set("TEST_VARIABLE", "value");
    tpau::cpp_kernal::SystemEnvironment::set("TEST_VARIABLE", "new_value", false);
    auto value = tpau::cpp_kernal::SystemEnvironment::get("TEST_VARIABLE");
    REQUIRE(value.has_value());
    REQUIRE(*value == "value");
    tpau::cpp_kernal::SystemEnvironment::unset("TEST_VARIABLE");
}

TEST_CASE("SystemEnvironment: is_set", "[system_environment]") {
    tpau::cpp_kernal::SystemEnvironment::set("TEST_VARIABLE", "value");
    REQUIRE(tpau::cpp_kernal::SystemEnvironment::is_set("TEST_VARIABLE"));
    tpau::cpp_kernal::SystemEnvironment::unset("TEST_VARIABLE");
    REQUIRE(!tpau::cpp_kernal::SystemEnvironment::is_set("TEST_VARIABLE"));
}
