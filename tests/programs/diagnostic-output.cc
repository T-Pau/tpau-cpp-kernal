#include <string>
#include <unordered_map>

#include "DiagnosticOutput.h"
#include "FileReader.h"
#include "Util.h"

using namespace tpau::cpp_kernal;

bool handle_read(const Location& location, std::string_view command, std::string_view arguments) {
    try {
        (void)FileReader::global.read(arguments);
    }
    catch (const std::exception& e) {
        DiagnosticOutput::global.error(location, "Failed to read file '{}': {}", arguments, e.what());
        return false;
    }
    return true;
}

bool handle_output(const Location& location, std::string_view command, std::string_view arguments) {
    auto message_location = Location();
    auto message = std::string(arguments);

    if (!arguments.empty() && arguments[0] == '{') {
        auto end = arguments.find('}');
        if (end == std::string_view::npos) {
            DiagnosticOutput::global.error(location, "No closing brace found in location");
            return false;
        }
        auto location_str = trim(arguments.substr(1, end - 1));
        message = trim(arguments.substr(end + 1));
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
                    return false;
                }
            }
            if (location_parts.size() > 2) {
                try {
                    start_column = std::stoul(std::string(location_parts[2]));
                }
                catch (...) {
                    DiagnosticOutput::global.error(location, "Invalid start column in location: '{}'", location_str);
                    return false;
                }
            }
            if (location_parts.size() > 3) {
                try {
                    width = std::stoul(std::string(location_parts[3]));
                }
                catch (...) {
                    DiagnosticOutput::global.error(location, "Invalid width in location: '{}'", location_str);
                    return false;
                }
            }
            if (location_parts.size() > 4) {
                DiagnosticOutput::global.error(location, "Invalid location: '{}'", location_str);
                return false;
            }
            message_location = Location(file_name, line_number, start_column, start_column + width);
        }
    }

    if (message.empty()) {
        DiagnosticOutput::global.error(location, "No message specified");
        return false;
    }

    if (command == "notice") {
        DiagnosticOutput::global.notice(message_location, message);
    }
    else if (command == "warning") {
        DiagnosticOutput::global.warning(message_location, message);
    }
    else if (command == "error") {
        DiagnosticOutput::global.error(message_location, message);
    }
    else {
        DiagnosticOutput::global.error(location, "Unknown command: '{}'", command);
        return false;
    }

    return true;
}

std::unordered_map<std::string, bool (*)(const Location& location, std::string_view command, std::string_view arguments)> command_handlers = {
    {"error", handle_output},
    {"notice", handle_output},
    {"read", handle_read},
    {"warning", handle_output},
};

int main() {
    auto line = std::string();
    auto line_number = 0;
    while (std::getline(std::cin, line)) {
        line_number += 1;
        if (line.empty()) {
            continue;
        }
        auto space_pos = line.find(' ');
        auto command = line.substr(0, space_pos);
        auto arguments = space_pos == std::string::npos ? "" : trim(line.substr(space_pos + 1));
        auto location = Location("<stdin>", line_number, 0, 0);

        auto handler_it = command_handlers.find(std::string(command));
        if (handler_it == command_handlers.end()) {
            DiagnosticOutput::global.error(location, "unknown command: '{}'", command);
            return 2;
        }

        try {
            if (!handler_it->second(location, command, arguments)) {
                return 2;
            }
        }
        catch (const std::exception& e) {
            DiagnosticOutput::global.error(location, "command caused uncaught exception: {}", e.what());
            return 2;
        }
    }

    if (DiagnosticOutput::global.failed()) {
        return 1;
    }

    return 0;
}