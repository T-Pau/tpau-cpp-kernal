# cspell:ignore INCLUDEDIR

set(KERNAL_SOURCES
        Base64Coder.cc
        Coder.cc
        Commandline.cc
        Command.cc
        DiagnosticOutput.cc
        Exception.cc
        FileReader.cc
        FileSource.cc
        HexCoder.cc
        Location.cc
        Int.cc
        Path.cc
        Symbol.cc
        Util.cc
        Value.cc
)

set(KERNAL_PUBLIC_HEADERS
        Base64Coder.h
        Coder.h
        Commandline.h
        Command.h
        DiagnosticOutput.h
        Exception.h
        FileReader.h
        FileSource.h
        HexCoder.h
        Int.h
        Location.h
        Path.h
        printf_like.h
        Symbol.h
        Util.h
        Value.h
)

add_library(tpau-cpp-kernal ${KERNAL_SOURCES})
add_library(tpau-cpp-kernal::tpau-cpp-kernal ALIAS tpau-cpp-kernal)

target_include_directories(tpau-cpp-kernal
        PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/tpau-cpp-kernal>
        PRIVATE
                ${PROJECT_BINARY_DIR}
)

install(TARGETS tpau-cpp-kernal
        EXPORT ${PROJECT_NAME}-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tpau-cpp-kernal
)

install(FILES ${KERNAL_PUBLIC_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tpau-cpp-kernal
)
