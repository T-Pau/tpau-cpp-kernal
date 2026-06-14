include(Catch)

set(TEST_PROGRAMS
    test-base64
    test-symbol
)

foreach(test_program IN LISTS TEST_PROGRAMS)
    add_executable(${test_program} ${test_program}.cc)
    target_include_directories(${test_program} PRIVATE BEFORE ${PROJECT_SOURCE_DIR}/tpau-cpp-kernal)
    target_link_libraries(${test_program} PRIVATE tpau-cpp-kernal Catch2::Catch2WithMain)
    catch_discover_tests(${test_program})
endforeach()

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
