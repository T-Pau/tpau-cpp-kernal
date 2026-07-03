#include "Command.h"
#include "Location.h"

#include <fstream>
#include <iostream>
#include <optional>

using namespace tpau::cpp_kernal;

/**
 * Class implementing a test program that reads command from a file or stdin and executes the corresponding command handlers.
 */
class TestCommand : public Command {
  public:
    /**
     * Processing status.
     */
    enum class Status {
        OK = 0,     ///< Processing succeeded.
        FAILED = 1, ///< Processing failed.
        ERROR = 2,  ///< An error occurred while processing.
    };

    /**
     * Class for parsing command arguments from a string.
     */
    class Arguments {
      public:
        /**
         * Create an Arguments object with the given arguments string.
         *
         * @param arguments The arguments string.
         */
        explicit Arguments(std::string arguments) : arguments(std::move(arguments)), remaining(this->arguments) {}
        Arguments(std::string_view arguments) : Arguments(std::string(arguments)) {}

        /**
         * Get the next word from the remaining arguments, removing it from the remaining arguments.
         *
         * @return The next word from the remaining arguments.
         */
        std::string_view next_word() { return *next_word(false); }

        /**
         * Get the next word from the remaining arguments, removing it from the remaining arguments.
         *
         * @param optional Whether the next word is optional.
         * @return The next word from the remaining arguments, or {} if it is optional and not present.
         */
        std::optional<std::string_view> next_word(bool optional);

        /**
         * Get the last word from the remaining arguments, removing it from the remaining arguments.
         *
         * @return The last word from the remaining arguments.
         */
        std::string_view last_word() { return *last_word(false); }

        /**
         * Get the last word from the remaining arguments, removing it from the remaining arguments.
         *
         * @param optional Whether the last word is optional.
         * @return The last word from the remaining arguments, or {} if it is optional and not present.
         */
        std::optional<std::string_view> last_word(bool optional);

        /**
         * Get the enclosed substring from the beginning of the remaining arguments, removing it from the remaining arguments.
         *
         * @param prefix The opening string.
         * @param suffix The closing string.
         * @return The enclosed substring.
         */
        std::optional<std::string_view> enclosed(std::string_view prefix, std::string_view suffix) { return *enclosed(prefix, suffix, false); }

        /**
         * Get the enclosed substring from the beginning of the remaining arguments, removing it from the remaining arguments.
         *
         * @param prefix The opening string.
         * @param suffix The closing string.
         * @param optional Whether the enclosed substring is optional.
         * @return The enclosed substring, or {} if it is optional and not present.
         */
        std::optional<std::string_view> enclosed(std::string_view prefix, std::string_view suffix, bool optional);

        /**
         * Get all remaining arguments.
         *
         * @return All remaining arguments.
         */
        std::string_view all();

      private:
        /// @brief The original arguments string.
        std::string arguments;

      public:
        /// @brief The remaining arguments.
        std::string_view remaining;
    };

    typedef TestCommand::Status (*CommandHandler)(const Location& location, std::string_view command, Arguments arguments);


    class CommandDefinition {
      public:
        CommandDefinition(std::string arguments, std::string description, TestCommand::CommandHandler handler) : arguments(std::move(arguments)), description(std::move(description)), handler(handler) {}

        std::string arguments;
        std::string description;
        TestCommand::CommandHandler handler;
    };

    /**
     * Create a TestCommand object.
     *
     * @param name The name of the program.
     * @param commands A map of command names to their corresponding command definitions.
     */
    TestCommand(std::string_view name, std::unordered_map<std::string, CommandDefinition> commands) : Command(options, "[command-file]", name), commands(std::move(commands)) {}

  protected:
    int process() override;
    size_t maximum_arguments() override { return 1; }

  private:
    /**
     * Set the processing status to a new value. The status can only be increased.
     */
    void set_status(Status new_status) {
        if (new_status > status) {
            status = new_status;
        }
    }

    /**
     * Show the available commands with their arguments and descriptions.
     */
    void show_commands() const;

    /// @brief The current processing status of the program.
    Status status{Status::OK};

    /// @brief The command handlers.
    std::unordered_map<std::string, CommandDefinition> commands;

    std::ifstream file_stream;
    std::istream* input_stream{&std::cin};

    static std::vector<Commandline::Option> options;
};
