#include <Coder.h>
#include <Exception.h>

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

void CoderEngine::encode(uint8_t datum) {
    if (finished) {
        throw Exception("encoding already finished");
    }
    if (error) {
        throw Exception("can't encode after error");
    }
    encode_implementation(datum);
}

void CoderEngine::finish() {
    if (finished) {
        throw Exception("encoding already finished");
    }
    if (error) {
        throw Exception("can't finish after error");
    }
    finish_implementation();
    finished = true;
}

void Coder::encode(const std::string& data) {
    for (auto c : data) {
        encode(static_cast<uint8_t>(c));
    }
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
