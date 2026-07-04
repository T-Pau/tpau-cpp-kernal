#ifndef HAD_TPAU_CPP_KERNAL_COMMANDLINE_H
#define HAD_TPAU_CPP_KERNAL_COMMANDLINE_H

/*
Copyright (C) Dieter Baron and Thomas Klausner

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

#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace tpau::cpp_kernal {
#if 0  
} // fix auto-indentation
#endif

/**
 * The parsed command line options and arguments.
 */
class ParsedCommandline {
  public:
    /**
     * A parsed command line option.
     */
    class OptionValue {
      public:
        /**
         * Create a parsed command line option with the given name and argument.
         *
         * @param name The name of the option.
         * @param argument The argument of the option. Empty string for options without argument.
         */
        OptionValue(std::string name_, std::string argument_) : name(std::move(name_)), argument(std::move(argument_)) {}

        /**
         * The name of the option.
         */
        std::string name;

        /**
         * The argument of the option. Empty string for options without argument.
         */
        std::string argument;
    };

    /**
     * Find the first occurrence of the option with the given name.
     *
     * @param name The name of the option to find.
     *
     * @return The argument of the option, or `{}` if the option was not found.
     */
    [[nodiscard]] std::optional<std::string> find_first(std::string_view name) const;

    /**
     * Find the last occurrence of the option with the given name.
     *
     * @param name The name of the option to find.
     *
     * @return The argument of the option, or `{}` if the option was not found.
     */
    [[maybe_unused]] [[nodiscard]] std::optional<std::string> find_last(std::string_view name) const;

    /**
     * Add an argument to the parsed command line.
     *
     * @param name The argument to add.
     */
    void add_argument(std::string name) { arguments.emplace_back(std::move(name)); }

    /**
     * Add an option to the parsed command line.
     *
     * @param name The name of the option.
     * @param argument The argument of the option. Empty string for options without argument.
     */
    void add_option(std::string name, std::string argument) { options.emplace_back(std::move(name), std::move(argument)); }

    /**
     * The parsed command line options.
     */
    std::vector<OptionValue> options;

    /**
     * The parsed command line arguments.
     */
    std::vector<std::string> arguments;

    /**
     * The exit code of the command. If this is set, the command should not be run and the program should exit with this code.
     */
    std::optional<int> exit_code;
};

/**
 * A command line parser.
 *
 * It parses command line options and arguments according to the given options and arguments specification. It also provides usage and version information.
 */
class Commandline {
  public:
    /**
     * A command line option.
     */
    class Option {
      public:
        /**
         * Create a command line option with both short and long variants and an argument.
         *
         * @param name The long name of the option. This is specified on the command line by prefixing it with `--` (e.g. "help" for `--help`).
         * @param short_name The short name of the option. This is specified on the command line by prefixing it with `-` (e.g. 'h' for `-h`).
         * @param argument_name The name of the argument for the option. This is used in the usage message.
         * @param description The description of the option. This is used in the usage message.
         */
        Option(std::string name_, char short_name_, std::string argument_name_, std::string description_) : name(std::move(name_)), short_name(short_name_), argument_name(std::move(argument_name_)), description(std::move(description_)) {}

        /**
         * Create a command line option with a long variant and an argument.
         *
         * @param name The long name of the option. This is specified on the command line by prefixing it with `--` (e.g. "help" for `--help`).
         * @param argument_name The name of the argument for the option. This is used in the usage message.
         * @param description The description of the option. This is used in the usage message.
         */
        Option(std::string name_, std::string argument_name_, std::string description_) : name(std::move(name_)), argument_name(std::move(argument_name_)), description(std::move(description_)) {}

        /**
         * Create a command line option with both short and long variants and no argument.
         *
         * @param name The long name of the option. This is specified on the command line by prefixing it with `--` (e.g. "help" for `--help`).
         * @param short_name The short name of the option. This is specified on the command line by prefixing it with `-` (e.g. 'h' for `-h`).
         * @param description The description of the option. This is used in the usage message.
         */
        Option(std::string name_, char short_name_, std::string description_) : name(std::move(name_)), short_name(short_name_), description(std::move(description_)) {}

        /**
         * Create a command line option with a long variant and no argument.
         *
         * @param name The long name of the option. This is specified on the command line by prefixing it with `--` (e.g. "help" for `--help`).
         * @param description The description of the option. This is used in the usage message.
         */
        Option(std::string name_, std::string description_) : name(std::move(name_)), description(std::move(description_)) {}

        /// @brief The long name of the option. This is specified on the command line by prefixing it with `--` (e.g. "help" for `--help`).
        std::string name;

        /// @brief The short name of the option. This is specified on the command line by prefixing it with `-` (e.g. 'h' for `-h`). Empty if the option has no short name.
        std::optional<char> short_name;

        /// @brief The name of the argument for the option. This is used in the usage message. Empty if the option has no argument.
        std::string argument_name;

        /// @brief The description of the option. This is used in the usage message.
        std::string description;

        /**
         * Check if the option has an argument.
         *
         * @return `true` if the option has an argument, `false` otherwise.
         */
        [[nodiscard]] bool has_argument() const { return !argument_name.empty(); }

        bool operator<(const Option& other) const;
    };

    /**
     * Create a command line parser.
     *
     * @param options The list of options supported by the command line parser.
     * @param arguments The description of the positional arguments. This is used in the usage message.
     * @param header The header of the usage message.
     * @param footer The footer of the usage message.
     * @param version The version of the program.
     */
    Commandline(std::vector<Option> options, std::string arguments, std::string header, std::string footer, std::string version);

    /**
     * Add an option to the command line parser.
     *
     * @param option The option to add.
     */
    void add_option(Option option);

    /// @brief The list of options supported by the command line parser.
    std::vector<Option> options;

    /// @brief The name of the program.
    std::string program_name;

    /// @brief The description of the positional arguments. This is used in the usage message.
    std::string arguments;

    /// @brief The header of the usage message.
    std::string header;

    /// @brief The footer of the usage message.
    std::string footer;

    /// @brief The version of the program.
    std::string version;

    /**
     * Parse the command line arguments and options.
     *
     * The command line arguments are expected as a list of C strings with `argv[0]` being the program name. This is the same format as the arguments passed to the `main()` function.
     *
     * @param argc The number of command line arguments.
     * @param argv The command line arguments.
     * @param allow_exit If `true`, the program will exit if the command should not run. Otherwise, exit_code will be set in the returned `ParsedCommandline`.
     * @return The parsed command line.
     */
    ParsedCommandline parse(int argc, char* const argv[], bool allow_exit = true);

    /**
     * Print the usage message to the given `FILE`.
     *
     * @param full If `true`, print the full usage message. If `false`, print a brief usage message.
     * @param fout The `FILE` to print the usage message to.
     */
    void usage(bool full = false, FILE* fout = stdout);

  private:
    bool options_sorted;

    void sort_options();
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_COMMANDLINE_H
