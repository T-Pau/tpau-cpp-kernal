#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include "FlagSet.h"

enum class Flag {
    A,
    B,
    C,
    D,
    E,
};


TEST_CASE("FlagSet: empty", "[flagset]") {
    auto empty = tpau::cpp_kernal::FlagSet<Flag>();
    REQUIRE(empty.empty());
    REQUIRE(!empty);
    REQUIRE(empty == tpau::cpp_kernal::FlagSet<Flag>());
    REQUIRE(!empty.is_enabled(Flag::A));
}

TEST_CASE("FlagSet: non-empty", "[flagset]") {
    auto a = tpau::cpp_kernal::FlagSet<Flag>(Flag::A);
    REQUIRE(!a.empty());
    REQUIRE(a);
    REQUIRE(a != tpau::cpp_kernal::FlagSet<Flag>());
    REQUIRE(a.is_enabled(Flag::A));
    REQUIRE(!a.is_enabled(Flag::B));
    REQUIRE(a[Flag::A]);
    REQUIRE(!a[Flag::B]);
}

TEST_CASE("FlagSet: same", "[flagset]") {
    auto a1 = tpau::cpp_kernal::FlagSet<Flag>(Flag::A);
    auto a2 = tpau::cpp_kernal::FlagSet<Flag>(Flag::A);
    REQUIRE(a1 == a2);
    REQUIRE(!(a1 != a2));
}

TEST_CASE("FlagSet: different", "[flagset]") {
    auto ab = tpau::cpp_kernal::FlagSet<Flag>(Flag::A, Flag::B);
    auto cd = tpau::cpp_kernal::FlagSet<Flag>(Flag::C, Flag::D);
    auto ac = tpau::cpp_kernal::FlagSet<Flag>(Flag::A, Flag::C);
    REQUIRE(ab != cd);
    REQUIRE(!(ab == cd));
    REQUIRE(!ab.any_enabled(cd));
    REQUIRE(!ab.all_enabled(cd));
    REQUIRE(ab.any_enabled(ac));
    REQUIRE(!ab.all_enabled(ac));
}

TEST_CASE("FlagSet: nested", "[flagset]") {
    auto ab = tpau::cpp_kernal::FlagSet<Flag>(Flag::A, Flag::B);
    auto abc = tpau::cpp_kernal::FlagSet<Flag>(ab, Flag::C);
    REQUIRE(abc.is_enabled(Flag::A));
    REQUIRE(abc.is_enabled(Flag::B));
    REQUIRE(abc.is_enabled(Flag::C));
    REQUIRE(!abc.is_enabled(Flag::D));
    REQUIRE(!abc.is_enabled(Flag::E));
    REQUIRE(abc.any_enabled(ab));
    REQUIRE(abc.all_enabled(ab));
}

