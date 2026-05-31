---
title: T'Pau C++ Kernal
description: A collection of C++ classes and functions used in T'Pau projects.
keywords: C++, library, collection, classes, functions, T'Pau
---
## What Is T'Pau C++ Kernal?

T'Pau C++ Kernal is a C++ library collecting various classes and functions that are used in other T'Pau projects. It is meant to be included as a Git submodule.

## Why Use T'Pau C++ Kernal?

T'Pau C++ Kernal was primarily developed for use in our own programs.
    
While the scripts creating assembly code, the provided assembler routines, and the build rules are specific to Accelerate and fast-ninja, some scripts and the underlying Python classes are more versatile.

## Getting Started

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

## Staying in Touch

If you found a problem, please [create an issue on GitHub](https://github.com/T-Pau/tpau-cpp-kernal/issues/new/choose) or let us know at [tpau-cpp-kernal@tpau.group](mailto:tpau-cpp-kernal@tpau.group).

Also let us know if the documentation is incomplete, inaccurate, or hard to understand.
