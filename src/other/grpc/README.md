# SikRPC Daemon

C++ implementation of the daemon component exposing the [Ethereum JSON RPC protocol](https://eth.wiki/json-rpc/API) within the [Turbo-Geth](https://github.com/ledgerwatch/turbo-geth) architecture.

[![CircleCI](https://circleci.com/gh/torquem-ch/silkrpc.svg?style=shield)](https://circleci.com/gh/torquem-ch/silkrpc)
[![Codecov master](https://img.shields.io/codecov/c/github/torquem-ch/silkrpc/master.svg?style=shield&logo=codecov&logoColor=white)](https://codecov.io/gh/torquem-ch/silkrpc)
![version](https://img.shields.io/github/v/release/torquem-ch/silkrpc?sort=semver&color=normal)
[![License](https://img.shields.io/github/license/torquem-ch/silkrpc?color=lightgrey)](https://img.shields.io/github/license/torquem-ch/silkrpc)
![semver](https://img.shields.io/badge/semver-2.0.0-blue)

<br>

# Building the source

# Clone the repository

```
git clone --recurse-submodules git@github.com:torquem-ch/silkrpc.git
```

To update the submodules later on run 
```
git submodule update --init --recursive
```

## Linux & MacOS
Building SikRPC Daemon requires:
* C++17 compiler (GCC or Clang)
* [CMake](http://cmake.org)
* [GMP](http://gmplib.org) (`sudo apt-get install libgmp3-dev` or `brew install gmp`)

Once the prerequisites are installed, bootstrap cmake by running
```
mkdir build
cd build
cmake ..
```
(You have to run `cmake ..` just the first time.)

Then run the build itself
```
make -j
```

Now you can run the unit tests
```
cmd/unit_test
```

## Windows
* Install [Visual Studio](https://www.visualstudio.com/downloads) 2019. Community edition is fine.
* Make sure your setup includes CMake support and Windows 10 SDK.
* Install [vcpkg](https://github.com/microsoft/vcpkg#quick-start-windows).
* `.\vcpkg\vcpkg install mpir:x64-windows`
* Add <VCPKG_ROOT>\installed\x64-windows\include to your `INCLUDE` environment variable.
* Add <VCPKG_ROOT>\installed\x64-windows\bin to your `PATH` environment variable.
* Open Visual Studio and select File -> CMake...
* Browse the folder where you have cloned this repository and select the file CMakeLists.txt
* Let CMake cache generation complete (it may take several minutes)
* Solution explorer shows the project tree.
* To build simply `CTRL+Shift+B`
* Binaries are written to `%USERPROFILE%\CMakeBuilds\silkrpc\build` If you want to change this path simply edit `CMakeSettings.json` file.

# Code style

We use the standard C++17 programming language. We follow the [Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html) with the following differences:

* `snake_case` for function names.
* .cpp & .hpp file extensions are used for C++; .cc are used just for [gRPC](https://grpc.io/) generated C++; .c & .h are reserved for C.
* `using namespace foo` is allowed inside .cpp files, but not inside headers.
* Exceptions are allowed.
* User-defined literals are allowed.
* Maximum line length is 120, indentation is 4 spaces – see `.clang-format`.


