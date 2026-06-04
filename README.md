## What Is T'Pau C++ Kernal?

T'Pau C++ Kernal is a C++ library collecting various classes and functions that are used in other T'Pau projects.

## Why Use T'Pau C++ Kernal?

T'Pau C++ Kernal was primarily developed for use in our own programs.
    
While the scripts creating assembly code, the provided assembler routines, and the build rules are specific to Accelerate and fast-ninja, some scripts and the underlying Python classes are more versatile.

## Getting Started

The T'Pau C++ Kernal provides CMake and pkg-config files for easy integration into your project.

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
