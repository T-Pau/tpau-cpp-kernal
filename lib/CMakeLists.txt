add_library(tpau-cpp-kernal
        Base64.cc
        Commandline.cc
        Command.cc
        DiagnosticOutput.cc
        Exception.cc
        FileReader.cc
        FileSource.cc
        Location.cc
        Int.cc
        Path.cc
        Symbol.cc
        Util.cc
        Value.cc
)

target_include_directories(tpau-cpp-kernal PRIVATE ${PROJECT_BINARY_DIR})

install(TARGETS tpau-cpp-kernal
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
