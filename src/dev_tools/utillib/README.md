## utillib

`utillib` provides various functionality in support of other libraries and applications that comprise TrueBlocks. This includes:

- support for data types such as signed and unsigned integers, floating point numbers, doubles, big numbers, time and date types
- methods for manipulating, searching, and processing strings
- collection types such as arrays, lists, and maps
- runtime typing
- serialization support
- a proprietary technology known as display strings
- concurrency protected file access
- screen, file, and string export and import functionality
- operating-system specific file and folder manipulations
- configuration file support
- command line option parsing
- performance timing support

## Future of this library

Much of the functionality provided by `utillib` is available in the standard C++ libraries, therefore, over time, this code will be removed. Most notably the string and collection classes will be removed soon. There are certain features which are required of any replacement to this library, features not provided in the standard C++ libraries, such as class hierarchy support (auto-creation and self-identification), serialization support, and the display string capability.

## Class hierarchy support

This feature makes available a number of capabilities including the ability to create instances of a class given a string input parameter. This feature makes possible the promotion of a blockchain’s transactions to the various token and per-smart-contract classes.

## Serialization

This feature allows for fast, binary storage and retrieval of the class data. This, along with significant pre-computations, is the primary way we speed up data access.

## Display strings

This feature allows TrueBlocks to export the state of a particular instance of a class in any desired format given an appropriately constructed formatting string including tab-delimited text, comma-separated values, JSON, or any other desired format including the ability to inject the data into an arbitrary database.
