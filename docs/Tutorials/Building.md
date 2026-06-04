# Building T'Pau C++ Kernal

## Prerequisites

To build T'Pau C++ Kernal, you will need to have a C++ compiler that supports C++20, and [CMake](https://cmake.org/) (version 3.10 or later). You can use any build system supported by CMake, such as Make, Ninja, or Visual Studio.

To run the tests, you will need to have [Catch2](https://github.com/catchorg/Catch2) installed. You can install it using your package manager, or you can build it from source.

## Building the Library

To build the library, follow these steps:

1. Clone the repository and navigate to the top level directory:
```bash
git clone https://github.com/T-Pau/tpau-cpp-kernal.git
cd tpau-cpp-kernal
```

2. Create a build directory and navigate to it:
```bash
mkdir build
cd build
```

3. Run CMake to configure the build system:
```bash
cmake ..
```

4. Build the library:
```bash
cmake --build .
```

5. Run the tests (optional):
```bash
ctest
```

## Installing the Library

You can use the library in other projects directly from the build directory. However, it is more convenient to install it to a system-wide location. 

To install the library, run the following command in the `build` directory:

```bash
sudo cmake --install .
```

This will install the library and its headers to the default system locations. You can specify a different installation prefix by adding the `-DCMAKE_INSTALL_PREFIX` option when running CMake:

```bash
cmake -DCMAKE_INSTALL_PREFIX=/path/to/install ..
```
