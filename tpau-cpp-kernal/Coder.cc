#include "Coder.h"

#include "Exception.h"

namespace tpau::cpp_kernal {

void CoderOutput::write(uint8_t datum) {
    if (ended) {
        throw Exception("output already ended");
    }
    write_implementation(datum);
}

void CoderOutput::end() {
    if (ended) {
        throw Exception("output already ended");
    }
    end_implementation();
    ended = true;
}

void CoderEngine::process(uint8_t datum) {
    if (finished) {
        throw Exception("encoding already finished");
    }
    if (error) {
        throw Exception("can't process after error");
    }
    process_implementation(datum);
}

void CoderEngine::finish() {
    if (finished) {
        throw Exception("processing already finished");
    }
    if (error) {
        throw Exception("can't finish after error");
    }
    finish_implementation();
    finished = true;
}

void Coder::process(std::string_view data) {
    for (auto c : data) {
        process(static_cast<uint8_t>(c));
    }
}

void StreamCoderOutput::write_implementation(uint8_t datum) {
    if (line_length != 0 && current_position >= line_length) {
        stream << std::endl << indent;
        current_position = 0;
    }
    stream << static_cast<char>(datum);
    current_position += 1;
}

void StringCoderOutput::write_implementation(uint8_t datum) {
    if (fragments.empty() || fragments.back().capacity() == fragments.back().size()) {
        fragments.emplace_back();
        fragments.back().reserve(fragment_size);
    }
    fragments.back() += static_cast<char>(datum);
}

std::string StringCoderOutput::get_string() {
    if (!fragments.empty()) {
        size_t total_size = 0;
        for (const auto& fragment : fragments) {
            total_size += fragment.size();
        }
        result.reserve(total_size);
        for (const auto& fragment : fragments) {
            result += fragment;
        }
        fragments.clear();
    }
    return result;
}

} // namespace tpau::cpp_kernal
