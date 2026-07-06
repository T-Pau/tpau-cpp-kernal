#ifndef HAD_TPAU_CPP_KERNAL_COMMAND_H
#define HAD_TPAU_CPP_KERNAL_COMMAND_H

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

#include <filesystem>
#include <limits>
#include <string>

#include "Commandline.h"
#include "Exception.h"
#include "FlagSet.h"

/**
 * Sets the default package information for the command line interface from CMake definitions.
 * When using CMake, this macro should be called in the main function of the program before creating a Command object, to pass the information from CMake to the command line interface.
 */
#define TPAU_CPP_KERNAL_SET_DEFAULT_PACKAGE_INFO()                               \
    tpau::cpp_kernal::Command::header = " -- " PACKAGE " by " PACKAGE_AUTHOR;    \
    tpau::cpp_kernal::Command::footer = "Report bugs to " PACKAGE_BUGREPORT "."; \
    tpau::cpp_kernal::Command::version = VERSION

namespace tpau::cpp_kernal {

/**
 * Base class for command line programs.
 *
 * It only produces the output files if no errors occurred during processing.
 */
class Command {
  public:
    /**
     * Optional features a subclass can enable.
     */
    enum class Feature : uint8_t {
        /**
         * The command creates an output file.
         *
         * This adds the `--output` command line option and the subclass must implement the `create_output()` method to create the output file.
         */
        OutputFile,

        /**
         * The command can create a gcc style dependency file.
         *
         * This option requires the `OutputFile` feature to be enabled.
         *
         * This adds the `--dependency-file` command line option.
         *
         * TODO: Implement the tracking of dependencies and the creation of the dependency file.
         */
        DependencyFile,
    };

    /**
     * Create a command line program with the given options and arguments.
     *
     * @param options The option definitions for the program.
     * @param arguments The argument names for the program.
     * @param name The name of the program.
     * @param features The features to enable for the program.
     */
    Command(const std::vector<Commandline::Option>& options, std::string arguments, std::string_view name, FlagSet<Feature> features = FlagSet<Feature>{});

    /**
     * Run the command line program with the given arguments.
     *
     * @param argc The number of arguments.
     * @param argv The arguments.
     * @return The exit code of the program.
     */
    int run(int argc, char* const argv[]);

    /// @brief The name of the program.
    std::string program_name;

  protected:
    /**
     * Process the command line arguments and options.
     *
     * This method must be implemented by subclasses to perform the actual processing of the command.
     *
     * @return A non-zero value indicates an error occurred during processing and will be used as the exit code.
     */
    virtual int process() = 0;

    /**
     * Create the output files for the command.
     *
     * If the feature `OutputFile` is enabled, this method must be implemented by subclasses to create the output files. It will only be called if no errors occurred during processing.
     *
     * @return A non-zero value indicates an error occurred during output creation and will be used as the exit code.
     */
    virtual int create_output() { throw Exception("create_output() not implemented"); }

    /**
     * Get the minimum number of arguments required for the command.
     *
     * This method can be overridden by subclasses.
     *
     * @return The minimum number of arguments required for the command. The default is 0.
     */
    virtual size_t minimum_arguments() { return 0; }

    /**
     * Get the maximum number of arguments allowed for the command.
     *
     * This method can be overridden by subclasses.
     *
     * @return The maximum number of arguments allowed for the command. The default is the maximum value of size_t, representing unlimited.
     */
    virtual size_t maximum_arguments() { return std::numeric_limits<size_t>::max(); }

    /**
     * The command line parser for the command.
     */
    Commandline commandline;

    /**
     * The parsed command line arguments and options.
     */
    ParsedCommandline arguments;

    /// @brief The name of the output file.
    std::optional<std::filesystem::path> output_file;

    /// @brief The name of the dependency file.
    std::optional<std::filesystem::path> dependency_file;

    /// @brief The features enabled for the command.
    FlagSet<Feature> features;

  private:
    /**
     * Create the dependency file for the command.
     *
     * Create the dependency file in gcc style format. This method will only be called if the feature `DependencyFile` is enabled and no errors occurred during processing.
     * 
     * It records all files read by the global FileReader as dependencies.
     *
     * TODO: Allow augmenting or overriding the list of dependencies in subclasses.
     *
     * @return A non-zero value indicates an error occurred during dependency file creation and will be used as the exit code.
     */
    virtual int create_dependency_file();

    static std::string header;
    static std::string footer;
    static std::string version;
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_COMMAND_H
