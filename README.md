# TrueBlocks Core

![GitHub repo size](https://img.shields.io/github/repo-size/scottydocs/README-template.md)
![GitHub contributors](https://img.shields.io/github/contributors/scottydocs/README-template.md)
![GitHub stars](https://img.shields.io/github/stars/TrueBlocks/trueblocks-core?style%3Dsocial)
![GitHub forks](https://img.shields.io/github/forks/TrueBlocks/trueblocks-core?style=social)
![Twitter Follow](https://img.shields.io/twitter/follow/trueblocks?style=social)

TrueBlocks is a collection of libraries, tools, and applications that improve access to the Ethereum data while remaining fully local. The data access is similar to the Ethereum RPC, but offers a number of improvements:

1) TrueBlocks scrapes the chain and builds a shareable index of address appearances. This index allows very fast access to transactional histories for a give address (something that is not available from the node itself),

2) TrueBlocks provides a blindingly-fast binary cache of data previously queried from the node. This speeds up subsequent access to the same data by an order of magintude. This speedup allows for a much better user experience for dApps including the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer),

3) TrueBlocks extends Ethereum's RPC interfaces. For example, you may query blocks and transactions by date, by range, by hashes and other identifiers. Furthermore, two additional endpoints are provided for extracting and listing historical transactions per address.

## Prerequisites

---

### Golang

Follow [these instructions](https://golang.org/doc/install) for installing `golang` on your system.

### CMake and the C++ Build Enviornment

Complete these commands to install `git`, `cmake`, and `clang-format`:

#### On Linux:

    sudo apt install build-essential git cmake python python-dev libcurl3-dev clang-format jq

#### On Mac:

    brew install cmake
    brew install git
    brew install clang-format
    brew install jq

We recommend that you run MacOS Big Sur or later for best results on the Mac.

#### On Windows:

TrueBlocks does not currently support Windows builds.

## Building TrueBlocks

---

You must build TrueBlocks from source in order to use it. (There is no installer.) Complete the pre-requisites above, and then follow these instructions:

    git clone -b develop git@github.com:TrueBlocks/trueblocks-core.git
    cd trueblocks-core
    mkdir build && cd build
    cmake ../src
    make

The above commands create a collection of executables in the `./trueblocks-core/bin` folder. Add this path to your shell's `$PATH`. (The following instructions assume you've done this.)

## Testing Your Installation

---

Run this command to test your installation:

    chifra --version

If this returns a valid version string, your installation is properly installed.

If the above command does not return a valid version string, you most likely skipped adding the `./bin` folder to your $PATH.

Make sure the above command works before proceeding.

## Connecting to an Ethereum Node

---

The next step is to try to retrieve data from your Ethereum Node. Type this command:

    chifra blocks 100

If the command returns valid JSON data, you're system is working correctly.

If not, you may see a message similar to the following:

    **Warning:** Your RPC endpoint (http://localhost :8545) was not found. To fix this problem edit a file...

Follow the instruction in that message to configure TrueBlocks to use your RPC provider (such as Infura, QuickNode, or your own dAppNode).

When completed, your configuration file should contain entries similar to this:

```[toml]
[settings]
rpcProvider = "http://localhost:8545"
# Make sure to use your own RPC provider
```

Continue until the above `chifra blocks` command returns valid JSON data.

## Building the TrueBlocks Index of Appearances

---

The primary data structure produced by TrueBlocks is an index of address appearances. This index provides very quick access to transaction histories for a given address.

You may either build the entire index from scratch (requires a tracing, archive node), or you may download part of the index and build it from there on.

This process is described in the TrueBlocks Recipe.

## What Exactly is TrueBlocks?

---

TrueBlocks creates an index of address appearances that allows you access the Ethereum data much more quickly. It also includes a large number of tools and examples that let you chain together queries on Ethereum data including transaction, addresses, blocks, ABIs, et cetera.

TrueBlocks also provides a heavy binary cache on your local machine. This makes repeated queries for the same data *very fast*. For large complex queries, this cache can reduce query times from minutes to seconds.

## Using TrueBlocks

---

The `chifra` tool is the basis for all other parts of TrueBlocks.
`chifra` helps you decipher chain data as well as manage various aspects of the system.

Please see the `chifra` README for more information.

## Contributing to TrueBlocks

---
We love contributors. Please see information about our [work flow](./docs/BRANCHING.md) before proceeding.

1. Fork this repository into your own repo.
2. Create a branch: `git checkout -b <branch_name>`.
3. Make changes to your local branch and commit them to your forked repo: `git commit -m '<commit_message>'`
4. Push back to the original branch: `git push origin TrueBlocks/trueblocks-core`
5. Create the pull request.

## Contributors

---
Thanks to the following people who have contributed to this project:

* [@tjayrush](https://github.com/tjayrush)
* [@crodnun](https://github.com/crodnun)
* [@wildmolasses](https://github.com/wildmolasses)
* [@MattDodsonEnglish](https://github.com/MattDodsonEnglish)

## Contact

---
If you have specific requests, contact us here <info@quickblocks.io>.
