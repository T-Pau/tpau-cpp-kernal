# Using T'Pau C++ Kernal in a CMake Project

This guide shows how to use T'Pau C++ Kernal in a CMake project. 

You can use an installed version of the library, or you can use it directly from its build directory. 

If you don't have the library yet, please refer to the [Building the Library](../Tutorials/Building/) tutorial.

## Adding the Library to Your CMake Project

If you're using an installed version of the library, add the following to your top level `CMakeLists.txt`:

```cmake
FIND_PACKAGE(tpau-cpp-kernal REQUIRED)
```

!!! todo

    Add instructions for using the library from its build directory.

!!! todo

    Add instructions for adding the include directory if it is not in the compiler's default search path.

## Using the Library in Your Code

All headers are in the `tpau-cpp-kernal` directory. For example, to use the `Value` class, include the following header:

```cpp
#include <tpau-cpp-kernal/Value.hpp>
```

All classes and functions are in the `tpau::cpp_kernal` namespace. For example, to use the `Value` class, you can write:

```cpp
tpau::cpp_kernal::Value value(42);
```

## Link against the Library

To link against the library, add the following to your target's `CMakeLists.txt`:

```cmake
TARGET_LINK_LIBRARIES(your-target tpau-cpp-kernal::tpau-cpp-kernal)
```
