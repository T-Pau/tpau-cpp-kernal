## What Is T'Pau C++ Kernal?

T'Pau C++ Kernal is a C++ library collecting various classes and functions that are used in other T'Pau projects. It is meant to be included as a Git submodule.

## Why Use T'Pau C++ Kernal?

T'Pau C++ Kernal was primarily developed for use in our own programs.
    
While the scripts creating assembly code, the provided assembler routines, and the build rules are specific to Accelerate and fast-ninja, some scripts and the underlying Python classes are more versatile.

## Getting Started

**This information is outdated. Proper documentation will be provided when cmake files for integrating the library are available.**


Include T'Pau C++ Kernal as a submodule in your Git repository:

``` sh
git submodule add https://github.com/T-Pau/tpau-cpp-kernal tpau-cpp-kernal
```

Add the following lines to your CMakeLists.txt:

``` cmake
add_subdirectory(tpau-cpp-kernal)
add_subdirectory(tpau-cpp-kernal/lib)
set(PACKAGE_AUTHOR "<author name>")
set(PACKAGE_BUGREPORT "<bugreport email address>")
```

For the programs using the library, add:

``` cmake
target_include_directories(${PROGRAM} PRIVATE ${CMAKE_SOURCE_DIR}/tpau-cpp-kernal/lib ${PROJECT_BINARY_DIR})
target_link_libraries(${PROGRAM} tpau-cpp-kernal)
```

## Building C++ Kernal

Kernal is written in C++20.

To build it, you need a C++20 compiler and [cmake](https://cmake.org). To run the tests, you need [Catch2](https://github.com/catchorg/Catch2).

To build the library, run the following commands in the top level directory:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

To run the tests, run the following command in the `build` directory:

```bash
ctest
```

## Staying in Touch

If you found a problem, please [create an issue on GitHub](https://github.com/T-Pau/tpau-cpp-kernal/issues/new/choose) or let us know at [tpau-cpp-kernal@tpau.group](mailto:tpau-cpp-kernal@tpau.group).

Also let us know if the documentation is incomplete, inaccurate, or hard to understand.
