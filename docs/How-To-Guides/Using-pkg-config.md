# Using T'Pau C++ Kernal in a Project with pkg-config

This guide shows how to use T'Pau C++ Kernal in a project with pkg-config. We'll use `make` as an example build system, but the general principles apply to any build system.

You can use an installed version of the library, or you can use it directly from its build directory. 

If you don't have the library yet, please refer to the [Building the Library](../Tutorials/Building/) tutorial.


## Adding the Library to Your Project with pkg-config

To find the library and its dependencies with pkg-config, add the following to your `Makefile`:

```make
CXXFLAGS += $(shell pkg-config --cflags tpau-cpp-kernal)
LDFLAGS += $(shell pkg-config --libs tpau-cpp-kernal)
```

This will link all programs against the library and its dependencies. How to add it only for certain targets depends on your build system and is beyond the scope of this guide, but in general you can add the output of `pkg-config` to the compiler and linker flags for those targets instead of `LDFLAGS`.

If you are using the library from its build directory, you will have to tell pkg-config where to find the configuration file and use the package name `tpau-cpp-kernal-uninstalled`. It is in the top level of the build directory.

```make
TPAU_CPP_KERNAL_DIR = /path/to/tpau-cpp-kernal/build
CXXFLAGS += $(shell pkg-config --with-path=$(TPAU_CPP_KERNAL_DIR) --cflags tpau-cpp-kernal-uninstalled)
LDFLAGS += $(shell pkg-config --with-path=$(TPAU_CPP_KERNAL_DIR) --libs tpau-cpp-kernal-uninstalled)
```


## Using the Library in Your Code

All headers are in the `tpau-cpp-kernal` directory. For example, to use the `Value` class, include the following header:

```cpp
#include <tpau-cpp-kernal/Value.hpp>
```

All classes and functions are in the `tpau::cpp_kernal` namespace. For example, to use the `Value` class, you can write:

```cpp
tpau::cpp_kernal::Value value(42);
```

