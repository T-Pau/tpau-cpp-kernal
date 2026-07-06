# Reference

This section contains reference documentation for the public API of T'Pau C++ Kernal. It is meant to be comprehensive and detailed, but may not be the best place to start for new users. It is intended to be used as a reference for specific classes and functions.

!!! warning
    The API is still in development and may change in breaking ways. 

The concepts and design decisions behind the API are explained in the [Explanations](../Explanations/) section. 

To get started, please refer to the [Tutorials](../Tutorials/) section. For practical instructions, see the [How-To Guides](../How-To-Guides/) section.


## [Base64Coder](Base64Coder/)

Encode and decode Base64 data to strings or output streams.


## [Coder](Coder/)

The base class for all coders, providing the framework to encode and decode data.


## [Command](Command/)

Base class for command line programs.


## [CommandLine](CommandLine/)

Parse command line arguments and options.


## [DiagnosticOutput](DiagnosticOutput/)

Output diagnostic messages, with optional location and source information in a format suitable for parsing by IDEs.


## [Exception](Exception/)

The base class for all exceptions thrown by T'Pau C++ Kernal. It allows adding system errors to the exception message.


## [FileReader](FileReader/)

Read text or binary files.


## [FileSource](FileSource/)

Read text files character by character, with support for peeking and ungetting characters, and resetting to a specific location.


## [FlagSet](FlagSet/)

A set of flags. The possible flags are defined by an enum type.


## [HexCoder](HexCoder/)

Encode and decode hexadecimal data to strings or output streams.


## [Int](Int/)

This class provides static methods for various integer utility functions.


## [Location](Location/)

A location in a source file, consisting of a file name and a range of line and column numbers.


## [SearchPath](SearchPath/)

Search files in a list of directories.


## [Symbol](Symbol/)

A cheap and small representation of a string.


## [SystemEnvironment](SystemEnvironment/)

Access to system environment variables.


## [UTF8](UTF8/)

Utility functions for working with UTF-8 and Unicode code-points.


## [Util](Util/)

This header file collects various utility functions.


## [Value](Value/)

Values of various types, and operations on them, with type and overflow checking.
