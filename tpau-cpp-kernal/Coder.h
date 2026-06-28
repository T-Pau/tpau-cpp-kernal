#ifndef HAD_TPAU_CPP_KERNAL_CODER_H
#define HAD_TPAU_CPP_KERNAL_CODER_H

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

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

namespace tpau::cpp_kernal {

class CoderOutput {
    // @brief A helper class to write coded data to an output.
  public:
    virtual ~CoderOutput() = default;

    /**
     * Write the given byte to the output.
     *
     * @param datum The byte to write.
     *
     * @throws std::runtime_error if an error occurs while writing the byte.
     * @throws Exception if the output has already been ended.
     */
    void write(uint8_t datum);

    /**
     * All output has been written.
     *
     * @throws std::runtime_error if an error occurs while ending the output.
     * @throws Exception if the output has already been ended.
     */
    void end();

  protected:
    /**
     * Write the given byte to the output. This is called by `write()`.
     *
     * @param datum The byte to write.
     *
     * @throws std::runtime_error if an error occurs while writing the byte.
     */
    virtual void write_implementation(uint8_t datum) = 0;
    /**
     * End the output. This is called by `end()`.
     *
     * @throws std::runtime_error if an error occurs while ending the output.
     */
    virtual void end_implementation() {}

    // Whether the output has already been ended.
    bool ended{false};
};


class CoderEngine {
    // @brief A helper class to code data and write it to a CoderOutput.
  public:
    CoderEngine(CoderOutput& output) : output(output) {}
    virtual ~CoderEngine() = default;

    /**
     * Process the given byte.
     *
     * @param datum The byte to process.
     */
    void process(uint8_t datum);

    /**
     * Finish the coding process.
     */
    void finish();

  protected:
    void write(uint8_t datum) { output.write(datum); }
    void end() { output.end(); }

    CoderOutput& output;

    /**
     * Process the given byte.
     *
     * @param datum The byte to process.
     */
    virtual void process_implementation(uint8_t datum) = 0;

    /**
     * Finish the coding process.
     */
    virtual void finish_implementation() {}

    bool finished{false};
    bool error{false};
};


class Coder {
    // @brief A helper class to code data using a CoderEngine.
  public:
    virtual ~Coder() = default;

    /**
     * Process the given data.
     *
     * @param data The data to process.
     */
    void process(std::string_view data);

    /**
     * Process the given byte.
     *
     * @param datum The byte to process.
     */
    void process(uint8_t datum) { engine.process(datum); }

  protected:
    Coder(CoderEngine& engine) : engine(engine) {}

    /**
     * Finish the coding process.
     *
     * Subclasses should call this method in their `end()` method.
     */
    void finish() { engine.finish(); }

  private:
    CoderEngine& engine;
};

class StreamCoderOutput : public CoderOutput {
    // @brief A CoderOutput that writes to a stream.
  public:
    StreamCoderOutput(std::ostream& stream, size_t line_length = 0, size_t indent = 0) : stream(stream), line_length(line_length), indent(std::string(indent, ' ')) {}
    void write_implementation(uint8_t datum) override;

  private:
    std::ostream& stream;
    size_t line_length;
    size_t current_position{0};
    std::string indent;
};

class StringCoderOutput : public CoderOutput {
    // @brief A CoderOutput that writes to a string.
  public:
    StringCoderOutput() = default;

    void write_implementation(uint8_t datum) override;

    /**
     * Get the string that has been written to the output.
     *
     * @return The string that has been written to the output.
     * @throws Exception if the output has not been ended yet.
     */
    std::string get_string();

  private:
    /// The output is stored in fragments with a reserved capacity.
    std::vector<std::string> fragments;

    /// @brief The final result string. This is calculated in the first call to `get_string()`.
    std::string result;

    /// @brief The size of each fragment.
    static const size_t fragment_size{1024};
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_CODER_H
