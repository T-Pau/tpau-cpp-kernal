#include "TestCommand.h"

#include <ranges>

#include "DiagnosticOutput.h"
#include "Exception.h"
#include "Util.h"

std::vector<Commandline::Option> TestCommand::options = {
    {"commands", "Show list of available commands"},
};

int TestCommand::process() {
    if (arguments.find_first("commands")) {
        show_commands();
        return static_cast<int>(Status::OK);
    }

    std::string_view filename;
    if (!arguments.arguments.empty()) {
        filename = arguments.arguments[0];
        file_stream.open(filename);
        if (!file_stream) {
            DiagnosticOutput::global.error(Location(filename), "Failed to open file '{}'", filename);
            return static_cast<int>(Status::ERROR);
        }
        input_stream = &file_stream;
    }
    else {
        input_stream = &std::cin;
        filename = "<stdin>";
    }
    auto line_number = 0;
    std::string raw_line;
    while (std::getline(*input_stream, raw_line)) {
        line_number += 1;

        auto line = trim(raw_line);
        if (line.empty()) {
            continue;
        }
        if (line[0] == '#') {
            continue;
        }

        auto space_pos = line.find(' ');
        auto command = line.substr(0, space_pos);
        auto arguments = space_pos == std::string::npos ? "" : trim(line.substr(space_pos + 1));
        auto location = Location(filename, line_number, 0, 0);

        auto handler_it = commands.find(std::string(command));
        if (handler_it == commands.end()) {
            DiagnosticOutput::global.error(location, "unknown command: '{}'", command);
            continue;
        }

        try {
            set_status(handler_it->second.handler(location, command, Arguments{arguments}));
        }
        catch (const std::exception& e) {
            DiagnosticOutput::global.error(location, "command caused uncaught exception: {}", e.what());
            set_status(Status::ERROR);
        }
    }

    return static_cast<int>(status);
}

std::string_view TestCommand::Arguments::all() {
    auto value = remaining;
    remaining = "";
    return value;
}

std::optional<std::string_view> TestCommand::Arguments::next_word(bool optional) {
    if (remaining.empty()) {
        if (optional) {
            return {};
        }
        throw Exception("missing next argument");
    }
    auto space_pos = remaining.find(' ');
    auto word = remaining.substr(0, space_pos);
    remaining = space_pos == std::string::npos ? "" : trim(remaining.substr(space_pos + 1));
    return word;
}

std::optional<std::string_view> TestCommand::Arguments::last_word(bool optional) {
    if (remaining.empty()) {
        if (optional) {
            return {};
        }
        throw Exception("missing last argument");
    }
    auto space_pos = remaining.find_last_of(' ');
    auto word = remaining.substr(space_pos + 1);
    remaining = space_pos == std::string::npos ? "" : trim(remaining.substr(0, space_pos));
    return word;
}

std::optional<std::string_view> TestCommand::Arguments::enclosed(std::string_view prefix, std::string_view suffix, bool optional) {
    if (remaining.empty()) {
        if (optional) {
            return {};
        }
        throw Exception("missing enclosed argument");
    }
    if (!remaining.starts_with(prefix)) {
        if (optional) {
            return {};
        }
        throw Exception("expected opening string '{}' in arguments", prefix);
    }
    auto close_pos = remaining.find(suffix);
    if (close_pos == std::string::npos) {
        throw Exception("missing closing string '{}' in arguments", suffix);
    }
    auto value = trim(remaining.substr(prefix.size(), close_pos - prefix.size()));
    remaining = trim(remaining.substr(close_pos + suffix.size()));
    return value;
}

void TestCommand::show_commands() const {
    std::cout << "Available commands:\n";
    size_t max_length = 0;
    for (const auto& [name, definition] : commands) {
        max_length = std::max(max_length, name.size() + 1 + definition.arguments.size());
    }

    for (const auto& name : sorted(commands | std::views::keys)) {
        const auto& definition = commands.at(name);
        std::string usage = name;
        if (!definition.arguments.empty()) {
            usage += " " + definition.arguments;
        }
        std::cout << "  " << std::setw(max_length) << std::left << usage;
        if (!definition.description.empty()) {
            std::cout << "  " << definition.description;
        }
        std::cout << "\n";
    }
}
