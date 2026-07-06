# cspell:ignore BUGREPORT distcheck DHAVE DCMAKE PYTHONBIN includedir strerrorlen putenv

CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

PROJECT(tpau-cpp-kernal
        VERSION 0.1
        LANGUAGES C CXX)
set(PACKAGE_BUGREPORT "tpau-cpp-kernal@tpau.group")

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

option(RUN_REGRESS "Run regression tests" ON)

include(CheckFunctionExists)
INCLUDE(GNUInstallDirs)

if(RUN_REGRESS)
    find_package(Catch2 3)
    if (NOT Catch2_FOUND)
        message(WARNING "-- Catch2 not found, unit testing disabled")
    endif()

    if (NOT PYTHONBIN)
        find_package (Python COMPONENTS Interpreter)
        set (PYTHONBIN ${Python_EXECUTABLE})
    else()
        message("Python set to ${PYTHONBIN}")
    endif()

    if (PYTHONBIN)
        find_program(NIHTEST nihtest)
        if (NOT NIHTEST)
            execute_process(
                COMMAND ${PYTHONBIN} -m nihtest --version
                OUTPUT_VARIABLE FOO
                RESULT_VARIABLE NIHTEST_MODULE
            )
            if (NIHTEST_MODULE EQUAL 0)
                set(NIHTEST "${PYTHONBIN}" "-m" "nihtest")
            endif()
        endif()
    endif(PYTHONBIN)

    set(USE_NIHTEST OFF)
    if (NOT NIHTEST OR NOT PYTHONBIN)
        message(WARNING "-- nihtest or Python interpreter not found, command testing disabled")
    else()
        set(NIHTEST_REQUIRED_VERSION "1.9.1")
        execute_process(
            COMMAND ${NIHTEST} --version
            OUTPUT_VARIABLE NIHTEST_VERSION_RAW
        )
        string(REGEX REPLACE "[^ ]* " "" NIHTEST_VERSION "${NIHTEST_VERSION_RAW}")
        if (NOT NIHTEST_VERSION)
            message(WARNING "-- can't get nihtest version, command testing disabled")
        elseif (${NIHTEST_VERSION} VERSION_LESS ${NIHTEST_REQUIRED_VERSION})
            message(WARNING "-- nihtest ${NIHTEST_VERSION} too old, at least ${NIHTEST_REQUIRED_VERSION} required, command testing disabled")
        else()
            set(USE_NIHTEST ON)
        endif()
    endif()
    if (NOT USE_NIHTEST AND NOT Catch2_FOUND)
        set(RUN_REGRESS OFF)
    endif()
endif()


check_function_exists(getenv_s HAVE_GETENV_S)
check_function_exists(GetEnvironmentVariableA HAVE_GET_ENVIRONMENT_VARIABLE_A)
check_function_exists(_putenv_s HAVE__PUTENV_S)
check_function_exists(strerror_s HAVE_STRERROR_S)
check_function_exists(strerrorlen_s HAVE_STRERRORLEN_S)
check_function_exists(SetEnvironmentVariableA HAVE_SET_ENVIRONMENT_VARIABLE_A)

set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION})
IF(NOT TARGET dist)
ADD_CUSTOM_TARGET(dist
  COMMAND git config tar.tar.xz.command "xz -c"
  COMMAND git archive --prefix=${ARCHIVE_NAME}/ -o ${ARCHIVE_NAME}.tar.gz HEAD
  COMMAND git archive --prefix=${ARCHIVE_NAME}/ -o ${ARCHIVE_NAME}.tar.xz HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
ADD_CUSTOM_TARGET(distcheck
  COMMAND chmod -R u+w ${ARCHIVE_NAME} ${ARCHIVE_NAME}-build ${ARCHIVE_NAME}-dest 2>/dev/null || true
  COMMAND rm -rf ${ARCHIVE_NAME} ${ARCHIVE_NAME}-build ${ARCHIVE_NAME}-dest
  COMMAND cmake -E tar xf ${ARCHIVE_NAME}.tar.gz
  COMMAND chmod -R u-w ${ARCHIVE_NAME}
  COMMAND mkdir ${ARCHIVE_NAME}-build
  COMMAND mkdir ${ARCHIVE_NAME}-dest
  COMMAND cd ${ARCHIVE_NAME}-build && cmake -DCMAKE_INSTALL_PREFIX=../${ARCHIVE_NAME}-dest ../${ARCHIVE_NAME}
  COMMAND cd ${ARCHIVE_NAME}-build && make -j4
  COMMAND cd ${ARCHIVE_NAME}-build && make test
  COMMAND cd ${ARCHIVE_NAME}-build && make install
#  COMMAND cd ${ARCHIVE_NAME}-build && make uninstall
#  COMMAND if [ `find ${ARCHIVE_NAME}-dest ! -type d | wc -l` -ne 0 ]; then echo leftover files in ${ARCHIVE_NAME}-dest; false; fi
  COMMAND cd ${ARCHIVE_NAME}-build && make clean
  COMMAND chmod -R u+w ${ARCHIVE_NAME} ${ARCHIVE_NAME}-build ${ARCHIVE_NAME}-dest
  COMMAND rm -rf ${ARCHIVE_NAME} ${ARCHIVE_NAME}-build ${ARCHIVE_NAME}-dest
  COMMAND echo "${ARCHIVE_NAME}.tar.gz is ready for distribution."
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
ADD_DEPENDENCIES(distcheck dist)
ENDIF(NOT TARGET dist)


# Checks

ADD_DEFINITIONS("-DHAVE_CONFIG_H")

# Testing
enable_testing()

# Targets
add_subdirectory(tpau-cpp-kernal)

if(RUN_REGRESS)
    add_subdirectory(tests)
endif(RUN_REGRESS)

# write out config file
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# pkgconfig file
file(RELATIVE_PATH pc_relative_libdir ${CMAKE_INSTALL_PREFIX} ${CMAKE_INSTALL_FULL_LIBDIR})
set(libdir "\${prefix}/${pc_relative_libdir}")
file(RELATIVE_PATH pc_relative_includedir ${CMAKE_INSTALL_PREFIX} ${CMAKE_INSTALL_FULL_INCLUDEDIR})
set(includedir "\${prefix}/${pc_relative_includedir}")
if(CMAKE_SYSTEM_NAME MATCHES BSD)
  set(PKG_CONFIG_RPATH "-Wl,-R\${libdir}")
endif(CMAKE_SYSTEM_NAME MATCHES BSD)
configure_file(tpau-cpp-kernal.pc.in tpau-cpp-kernal.pc @ONLY)
configure_file(tpau-cpp-kernal-uninstalled.pc.in tpau-cpp-kernal-uninstalled.pc @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/tpau-cpp-kernal.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

# CMake config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  COMPATIBILITY AnyNewerVersion)

configure_package_config_file("${PROJECT_NAME}-config.cmake.in" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

## Add targets to the build-tree export set
export(TARGETS tpau-cpp-kernal
  FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-targets.cmake")

# installation
install(FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  )
install(EXPORT ${PROJECT_NAME}-targets NAMESPACE tpau-cpp-kernal:: FILE ${PROJECT_NAME}-targets.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  )
