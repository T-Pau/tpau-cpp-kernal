#include "TestCommand.h"

#include "DiagnosticOutput.h"
#include "FileReader.h"
#include "Util.h"

using namespace tpau::cpp_kernal;

TestCommand::Status handle_read(const Location& location, std::string_view command, TestCommand::Arguments arguments) {
    auto filename = arguments.all();
    try {
        (void)FileReader::global.read(filename);
    }
    catch (const std::exception& e) {
        DiagnosticOutput::global.error(location, "Failed to read file '{}': {}", filename, e.what());
        return TestCommand::Status::ERROR;
    }
    return TestCommand::Status::OK;
}

TestCommand::Status handle_output(const Location& location, std::string_view command, TestCommand::Arguments arguments) {
    auto message_location = Location();

    if (auto optional_location_str = arguments.enclosed("{", "}", true)) {
        auto location_str = *optional_location_str;
        auto location_parts = split(location_str, " \t");
        if (!location_parts.empty()) {
            auto file_name = location_parts[0];
            auto line_number = 0;
            auto start_column = 0;
            auto width = 0;

            if (location_parts.size() > 1) {
                try {
                    line_number = std::stoul(std::string(location_parts[1]));
                }
                catch (...) {
                    DiagnosticOutput::global.error(location, "Invalid line number in location: '{}'", location_str);
                    return TestCommand::Status::ERROR;
                }
            }
            if (location_parts.size() > 2) {
                try {
                    start_column = std::stoul(std::string(location_parts[2]));
                }
                catch (...) {
                    DiagnosticOutput::global.error(location, "Invalid start column in location: '{}'", location_str);
                    return TestCommand::Status::ERROR;
                }
            }
            if (location_parts.size() > 3) {
                try {
                    width = std::stoul(std::string(location_parts[3]));
                }
                catch (...) {
                    DiagnosticOutput::global.error(location, "Invalid width in location: '{}'", location_str);
                    return TestCommand::Status::ERROR;
                }
            }
            if (location_parts.size() > 4) {
                DiagnosticOutput::global.error(location, "Invalid location: '{}'", location_str);
                return TestCommand::Status::ERROR;
            }
            message_location = Location(file_name, line_number, start_column, start_column + width);
        }
    }

    auto message = arguments.all();

    if (message.empty()) {
        DiagnosticOutput::global.error(location, "No message specified");
        return TestCommand::Status::ERROR;
    }

    if (command == "note") {
        DiagnosticOutput::global.note(message_location, message);
    }
    else if (command == "warning") {
        DiagnosticOutput::global.warning(message_location, message);
    }
    else if (command == "error") {
        DiagnosticOutput::global.error(message_location, message);
    }
    else {
        DiagnosticOutput::global.error(location, "Unknown command: '{}'", command);
        return TestCommand::Status::ERROR;
    }

    return TestCommand::Status::OK;
}

TestCommand::Status handle_mark_failed(const Location& location, std::string_view command, TestCommand::Arguments arguments) {
    DiagnosticOutput::global.mark_failed();

    return TestCommand::Status::OK;
}

std::unordered_map<std::string, TestCommand::CommandDefinition> command_definitions = {
    {"error", {"[{location}] message", "print error message", handle_output}}, {"mark_failed", {{}, "mark program as failed", handle_mark_failed}}, {"note", {"[{location}] message", "print note message", handle_output}}, {"read", {"filename", "read file", handle_read}}, {"warning", {"[{location}] message", "print warning message", handle_output}},
};

int main(int argc, char* const argv[]) {
    auto command = TestCommand("diagnostic-output", command_definitions);

    return command.run(argc, argv);
}
