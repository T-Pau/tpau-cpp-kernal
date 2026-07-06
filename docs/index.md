---
title: T'Pau C++ Kernal
description: A collection of C++ classes and functions used in T'Pau projects.
keywords: C++, library, collection, classes, functions, T'Pau
---
## What Is T'Pau C++ Kernal?

T'Pau C++ Kernal is a C++ library collecting various classes and functions that are used in other T'Pau projects. 

Its main focus is on command line utilities, especially development tools.


## Why Use T'Pau C++ Kernal?

T'Pau C++ Kernal was primarily developed for use in our own programs.

However, the provided classes form a solid foundation for building command line utilities, integrating with build system, and writing flexible parsers.    

!!! warning
    T'Pau C++ Kernal is still in early development and not yet stable. The API will change in breaking ways, and there are no official releases yet. Only use it if you are willing to adapt your code.


## Who Uses T'Pau C++ Kernal?

It is currently used in the cross assembler [Accelerate](https://accelerate.tpau.group) and the build system [fast-ninja](https://fast-ninja.tpau.group).


## Getting Started

Since there are no official releases yet, you will have to [build it from source](Tutorials/Building.md).

The T'Pau C++ Kernal can be easily [integrated into CMake projects](How-To-Guides/Using-cmake.md). It also provides [pkg-config files](How-To-Guides/Using-pkg-config.md) for other types of projects.


## Staying in Touch

If you found a problem, please [create an issue on GitHub](https://github.com/T-Pau/tpau-cpp-kernal/issues/new/choose) or let us know at [tpau-cpp-kernal@tpau.group](mailto:tpau-cpp-kernal@tpau.group).

Also let us know if the documentation is incomplete, inaccurate, or hard to understand.
