# cspell: words CTEST endforeach XFAIL STREQUAL

set(TEST_PROGRAMS
    test-base64
    test-flagset
    test-symbol
    test-system-environment
)

set(TEST_COMMAND_PROGRAMS
    command
    diagnostic-output
)

if(Catch2_FOUND)
    include(Catch)

    foreach(program IN LISTS TEST_PROGRAMS)
        add_executable(${program} programs/${program}.cc)
        target_include_directories(${program} PRIVATE BEFORE ${PROJECT_SOURCE_DIR}/tpau-cpp-kernal)
        target_link_libraries(${program} PRIVATE tpau-cpp-kernal Catch2::Catch2WithMain)
        catch_discover_tests(${program})
    endforeach()
endif(Catch2_FOUND)

if(USE_NIHTEST)
    foreach(program IN LISTS TEST_COMMAND_PROGRAMS)
        add_executable(${program} programs/${program}.cc programs/TestCommand.cc)
        target_include_directories(${program} PRIVATE BEFORE ${PROJECT_SOURCE_DIR}/tpau-cpp-kernal)
        target_link_libraries(${program} PRIVATE tpau-cpp-kernal)
    endforeach()

    file(GLOB TESTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.test)
    foreach(FULL_CASE IN LISTS TESTS)
        get_filename_component(CASE ${FULL_CASE} NAME)
        message(STATUS "Adding test ${CASE}")
        add_test(NAME ${CASE} COMMAND ${NIHTEST} -v ${CASE})
        set_tests_properties(${CASE} PROPERTIES SKIP_RETURN_CODE 77)
    endforeach()

    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/support/nihtest.conf.in ${CMAKE_CURRENT_BINARY_DIR}/nihtest.conf)
endif(USE_NIHTEST)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

