#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include "Symbol.h"


TEST_CASE("Symbol: empty", "[symbol]") {
    auto empty = tpau::cpp_kernal::Symbol();
    REQUIRE(empty.str() == "");
    REQUIRE(!empty);
    REQUIRE(empty.empty());
}

TEST_CASE("Symbol: non-empty", "[symbol]") {
    auto a = tpau::cpp_kernal::Symbol("a");
    REQUIRE(a.str() == "a");
    REQUIRE(a);
    REQUIRE(!a.empty());
    REQUIRE(a != tpau::cpp_kernal::Symbol());
}

TEST_CASE("Symbol: same", "[symbol]") {
    auto a1 = tpau::cpp_kernal::Symbol("a");
    auto a2 = tpau::cpp_kernal::Symbol("a");
    REQUIRE(a1 == a2);
    REQUIRE(a1 <= a2);
    REQUIRE(a1 >= a2);
    REQUIRE(!(a1 < a2));
    REQUIRE(!(a1 > a2));
    REQUIRE(!(a1 != a2));
}

TEST_CASE("Symbol: different", "[symbol]") {
    auto a = tpau::cpp_kernal::Symbol("a");
    auto b = tpau::cpp_kernal::Symbol("b");
    REQUIRE(a != b);
    REQUIRE(a < b);
    REQUIRE(a <= b);
    REQUIRE(b > a);
    REQUIRE(b >= a);
    REQUIRE(!(a == b));
    REQUIRE(!(a > b));
    REQUIRE(!(a >= b));
    REQUIRE(!(b < a));
    REQUIRE(!(b <= a));
}