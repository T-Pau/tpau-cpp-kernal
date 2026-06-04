#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include "Base64Coder.h"


TEST_CASE("Base64StringEncoder", "[base64]") {
    tpau::cpp_kernal::Base64StringEncoder encoder;
    encoder.process("Hello, World!");
    auto result = encoder.end();
    // cspell:disable-next-line
    REQUIRE(result == "SGVsbG8sIFdvcmxkIQ==");
}

TEST_CASE("Base64StreamEncoder", "[base64]") {
    std::stringstream output;
    tpau::cpp_kernal::Base64StreamEncoder encoder(output);
    encoder.process("Hello, World!");
    encoder.end();
    // cspell:disable-next-line
    REQUIRE(output.str() == "SGVsbG8sIFdvcmxkIQ==");
}

TEST_CASE("Base64StreamEncoder with line breaks and indent", "[base64]") {
    std::stringstream output;
    tpau::cpp_kernal::Base64StreamEncoder encoder(output, 4, 2);
    encoder.process("Hello, World!");
    encoder.end();
    // cspell:disable-next-line
    REQUIRE(output.str() == "SGVs\n  bG8s\n  IFdv\n  cmxk\n  IQ==");
}

TEST_CASE("Base64StringDecoder", "[base64]") {
    tpau::cpp_kernal::Base64StringDecoder decoder;
    // cspell:disable-next-line
    decoder.process("SGVsbG8sIFdvcmxkIQ==");
    auto result = decoder.end();
    REQUIRE(result == "Hello, World!");
}

TEST_CASE("Base64StringDecoder with whitespace", "[base64]") {
    tpau::cpp_kernal::Base64StringDecoder decoder;
    // cspell:disable-next-line
    decoder.process("SGVs\n  bG8s\n  IFdv\n  cmxk\n  IQ==");
    auto result = decoder.end();
    REQUIRE(result == "Hello, World!");
}

TEST_CASE("Base64StringDecoder with garbage after end marker", "[base64]") {
    tpau::cpp_kernal::Base64StringDecoder decoder;
    // cspell:disable-next-line
    REQUIRE_THROWS(decoder.process("SGVsbG8sIFdvcmxkIQ==garbage"), "garbage after end marker in base64 data");
}

TEST_CASE("Base64StringDecoder with unexpected end marker", "[base64]") {
    tpau::cpp_kernal::Base64StringDecoder decoder;
    // cspell:disable-next-line
    REQUIRE_THROWS(decoder.process("SGVsbG8sIFdvcmxkIQ==="), "unexpected end marker in base64 data");
}

TEST_CASE("Base64StringDecoder with illegal characters", "[base64]") {
    tpau::cpp_kernal::Base64StringDecoder decoder;
    // cspell:disable-next-line
    REQUIRE_THROWS(decoder.process("SGVsbG8$sIFdvcmxkI=="), "invalid character in base64 data");
}
