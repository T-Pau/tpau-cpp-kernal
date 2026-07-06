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

#include "Command.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <utility>

#include "DiagnosticOutput.h"
#include "FileReader.h"
#include "Util.h"

namespace tpau::cpp_kernal {

std::string Command::header = "";
std::string Command::footer = "";
std::string Command::version = "";

Command::Command(const std::vector<Commandline::Option>& options, std::string arguments, std::string_view name, FlagSet<Feature> features) : commandline(options, std::move(arguments), std::string(name) + header, footer, version), features(features) {
    if (features.is_enabled(Feature::OutputFile)) {
        commandline.add_option(Commandline::Option("output", 'o', "file", "write output to FILE"));
    }
    if (features.is_enabled(Feature::DependencyFile)) {
        commandline.add_option(Commandline::Option("depfile", 'M', "file", "write gcc-style dependency file to FILE"));
    }
}


int Command::run(int argc, char* const* argv) {
    try {
        program_name = argv[0];
        arguments = commandline.parse(argc, argv, false);
        if (arguments.exit_code) {
            return *arguments.exit_code;
        }

        if (arguments.arguments.size() < minimum_arguments() || arguments.arguments.size() > maximum_arguments()) {
            commandline.usage(false, stderr);
            return 1;
        }

        if (features.is_enabled(Feature::OutputFile)) {
            if (auto option = arguments.find_first("output")) {
                output_file = *option;
            }
        }

        if (features.is_enabled(Feature::DependencyFile)) {
            if (auto option = arguments.find_first("depfile")) {
                dependency_file = *option;
            }
        }

        auto ret = process();
        if (ret != 0) {
            return ret;
        }
        if (DiagnosticOutput::global.failed()) {
            return 1;
        }

        if (features.is_enabled(Feature::OutputFile)) {
            if (!output_file) {
                throw Exception("no output file specified");
            }
            auto result = create_output();
            if (result != 0) {
                (void)std::filesystem::remove(*output_file);
                return result;
            }

            if (dependency_file) {
                auto result = create_dependency_file();
                if (result != 0) {
                    (void)std::filesystem::remove(*output_file);
                    (void)std::filesystem::remove(*dependency_file);
                    return result;
                }
            }
        }
    }
    catch (std::exception& ex) {
        (void)std::filesystem::remove(*output_file);
        if (dependency_file) {
            (void)std::filesystem::remove(*dependency_file);
        }
        if (strlen(ex.what()) > 0) {
            std::cerr << program_name << ": " << ex.what() << std::endl;
        }
        return 1;
    }

    return 0;
}

int Command::create_dependency_file() {
    if (!dependency_file) {
        return 0;
    }

    auto depfile = std::ofstream(*dependency_file);
    if (!depfile) {
        throw Exception("can't create dependency file '{}': {}", dependency_file->string(), strerror_string());
    }

    depfile << *output_file << ":";
    for (const auto& file : FileReader::global.file_names()) {
        depfile << " " << file;
    }
    depfile << std::endl;

    depfile.close();
    if (!depfile) {
        throw Exception("can't write dependency file '{}': {}", dependency_file->string(), strerror_string());
    }

    return 0;
}

} // namespace tpau::cpp_kernal
