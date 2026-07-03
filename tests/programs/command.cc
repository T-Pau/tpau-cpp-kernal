#include "Command.h"
#include "DiagnosticOutput.h"

#include "TestCommand.h"

using namespace tpau::cpp_kernal;

class CommandSpecification {
  public:
    std::vector<Commandline::Option> options;
    std::string arguments;
    std::string name;
    FlagSet<Command::Feature> features;
    size_t minimum_arguments{0};
    size_t maximum_arguments{std::numeric_limits<size_t>::max()};

    // TODO: processing settings
};

class CommandTester : public Command {
  public:
    CommandTester(const CommandSpecification& specs) : Command(specs.options, specs.arguments, specs.name, specs.features), specs(specs) {}

  protected:
    int process() override {
        std::cout << "processing" << std::endl;
        return 0;
    }

    int create_output() override {
        if (specs.features.is_enabled(Feature::OutputFile)) {
            if (output_file) {
                std::cout << "creating output file '" << output_file->string() << "'" << std::endl;
            }
            else {
                std::cout << "creating default output file" << std::endl;
            }
        }
        return 0;
    }

    size_t minimum_arguments() override { return specs.minimum_arguments; }

    size_t maximum_arguments() override { return specs.maximum_arguments; }

  private:
    CommandSpecification specs;
};

CommandSpecification current_spec;

std::unordered_map<std::string, CommandTester> commands;

// Start a command specification
//      command <name> <arguments> {
TestCommand::Status start_command(const Location& location, std::string_view command, TestCommand::Arguments arguments) {
    if (arguments.last_word() != "{") {
        std::cerr << "Error: Expected '{' at the end of the command line." << std::endl;
        return TestCommand::Status::ERROR;
    }

    current_spec = CommandSpecification();
    current_spec.name = arguments.next_word();
    current_spec.arguments = arguments.all();

    return TestCommand::Status::OK;
}

// Add an option to the current command specification
//      option name [-short_name] [<argument_name>] description
TestCommand::Status add_option(const Location& location, std::string_view command, TestCommand::Arguments arguments) {
    auto name = arguments.next_word();
    auto argument_name = std::string_view();
    char short_name = '\0';

    if (auto short_name_word = arguments.enclosed("-", " ", true)) {
        if (short_name_word->size() != 1) {
            DiagnosticOutput::global.error(location, "short name must be a single character");
            return TestCommand::Status::ERROR;
        }
        short_name = (*short_name_word)[0];
    }

    if (auto maybe_argument_name = arguments.enclosed("<", ">", true)) {
        argument_name = *maybe_argument_name;
    }

    auto description = arguments.all();

    if (short_name != '\0' && !argument_name.empty()) {
        current_spec.options.emplace_back(std::string(name), short_name, std::string(argument_name), std::string(description));
    }
    else if (short_name != '\0') {
        current_spec.options.emplace_back(std::string(name), short_name, std::string(description));
    }
    else if (!argument_name.empty()) {
        current_spec.options.emplace_back(std::string(name), std::string(argument_name), std::string(description));
    }
    else {
        current_spec.options.emplace_back(std::string(name), std::string(description));
    }

    return TestCommand::Status::OK;
}

// Enable a feature for the current command specification
//      feature <feature_name>
TestCommand::Status enable_feature(const Location& location, std::string_view command, TestCommand::Arguments arguments) {
    auto feature_name = arguments.next_word();
    if (feature_name == "OutputFile") {
        current_spec.features.enable(Command::Feature::OutputFile);
    }
    else if (feature_name == "DependencyFile") {
        current_spec.features.enable(Command::Feature::DependencyFile);
    }
    else {
        DiagnosticOutput::global.error(location, "unknown feature: {}", feature_name);
        return TestCommand::Status::ERROR;
    }

    return TestCommand::Status::OK;
}

// End the current command specification
//      }
TestCommand::Status end_command(const Location& location, std::string_view command, TestCommand::Arguments arguments) {
    commands.emplace(current_spec.name, CommandTester(current_spec));
    current_spec = CommandSpecification();

    return TestCommand::Status::OK;
}

// Print mark to stdout and stderr
//      mark [string]
TestCommand::Status mark(const Location& location, std::string_view command, TestCommand::Arguments arguments) {
    std::string_view message = "------";
    auto custom_message = arguments.all();
    if (!custom_message.empty()) {
        message = custom_message;
    }
    std::cout << message << std::endl;
    std::cerr << message << std::endl;

    return TestCommand::Status::OK;
}

// Run a command with the given arguments
//      run command_name [arguments ...]
TestCommand::Status run_command(const Location& location, std::string_view command, TestCommand::Arguments arguments) {
    auto command_name = arguments.next_word();
    auto it = commands.find(std::string(command_name));
    if (it == commands.end()) {
        DiagnosticOutput::global.error(location, "unknown command: {}", command_name);
        return TestCommand::Status::ERROR;
    }
    auto& command_tester = it->second;

    // We need C-style strings for the command line arguments, so we convert the std::string_view to std::string and then to char*.
    std::vector<std::string> argv;
    argv.push_back(std::string(command_name));
    while (auto maybe_word = arguments.next_word(true)) {
        argv.push_back(std::string(*maybe_word));
    }
    std::vector<char*> argv_cstr;
    for (auto& arg : argv) {
        argv_cstr.push_back(arg.data());
    }

    int exit_code = command_tester.run(static_cast<int>(argv_cstr.size()), argv_cstr.data());
    std::cout << "exit code " << exit_code << std::endl;

    return TestCommand::Status::OK;
}

// clang-format off
std::unordered_map<std::string, TestCommand::CommandDefinition> command_definitions = {
    {"command", {"<name> <arguments> {", "start a command specification", start_command}}, 
    {"option", {"<name> [-short_name] [argument_name] description", "add an option to the current command specification", add_option}}, 
    {"feature", {"<feature>", "enable a feature for the current command specification", enable_feature}}, 
    {"}", {"", "end the current command specification", end_command}}, 
    {"mark", {"[string]", "print mark to stdout and stderr", mark}}, 
    {"run", {"command_name [arguments ...]", "run a command with the given arguments", run_command}},
};
// clang-format on

int main(int argc, char* const argv[]) {
    auto command = TestCommand("command", command_definitions);
    return command.run(argc, argv);
}
