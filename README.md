# TrueBlocks Core

![GitHub repo size](https://img.shields.io/github/repo-size/scottydocs/README-template.md)
![GitHub contributors](https://img.shields.io/github/contributors/scottydocs/README-template.md)
![GitHub stars](https://img.shields.io/github/stars/Great-Hill-Corporation/trueblocks-core?style%3Dsocial)
![GitHub forks](https://img.shields.io/github/forks/Great-Hill-Corporation/trueblocks-core?style=social)
![Twitter Follow](https://img.shields.io/twitter/follow/trueblocks?style=social)

TrueBlocks allows you to build local-first, fully-decentralized applications using data directly from an Ethereum node. It does this through two mechanisms:

1. A lightning-fast index of every appearance of every addresses on the chain, and
2. A binary cache of only the data your application extracts. 

Local-first means your application is **private by default**, the client-side cache means your application is **fast**, and *extraction-on-demand* means your application will remain **minimal**.

[How does TrueBlocks Work?](./docs/FAQ.md#how-it-works)

## Prerequisites
---

Before building TrueBlocks, you need to make sure you have `git`, `cmake`, `clang-format`, and `go` available.

Follow [these instructions](https://golang.org/doc/install) for installing `golang` on your system.

Complete these commands to install `git`, `cmake`, and `clang-format`:

#### On Linux:

```[shell]
sudo apt install build-essential git cmake python python-dev libcurl3-dev clang-format jq
```

#### On Mac:

```[shell]
brew install cmake
brew install git
brew install clang-format
brew install jq
```
## Building TrueBlocks

Currently, you must build TrueBlocks from source:

```[shell]
git clone git@github.com:TrueBlocks/trueblocks-core.git
cd trueblocks-core
mkdir build && cd build
cmake ../src
make
```

This will create a series of executables in the `./bin` folder at the top of the repo. The following instructions assume you've added this folder to your `$PATH`.

[Does TrueBlocks Work on Windows?](./docs/FAQ.md-windows)

## Testing Installation
---

After building TrueBlocks, you may test your configuration with this command:

```[shell]
chifra --version
```

For a complete list of available commands, run this command:

```[shell]
chifra --help
```

Next, let's see if you can get a block from your Ethereum node. Enter this command:

```[shell]
chifra blocks 1000
```

This should return valid JSON data for block 1,000 (type `getBlock --help` for more options on this tool.) If you get an error, such as this:

```[shell]
Warning: The Ethereum RPC: 'http://localhost:8545' was not found. Quitting...
```

You need to edit the file `~/.quickBlocks/quickBlocks.toml` to provide the URL of an available Ethereum RPC provider. Enter this text (adding the `settings` sections if it's not present):

```[toml]
[settings]
rpcProvider="<url-to-your-rpc-provider>
```

Once you get `chifra blocks` to return JSON data, you are ready to test your installtion. While optional, one of the tools (`ethslurp`) requires an Etherscan key. Get an EtherScan API key now, then run:

```[shell]
make tests
```

## Using TrueBlocks
---

The `chifra` tool is the basis for everything related to TrueBlocks. (`chifra` is derived from the Arabic word for `chipher`.) It helps you decipher chain data.

[Running the API](./docs/FAQ.md#running-the-api)

[Using the Command Line](./docs/FAQ.md#using-the-command-line)

[Using the Libraries](./docs/FAQ.md#using-the-library)

[Where to Go from Here...](./docs/FAQ.md#where-to-go-from-here)

[Full Documentation...](./docs/FAQ.md#full-documentation)

## Contributing to TrueBlocks
---
Please see information about our [work flow](./docs/BRANCHING.md) before proceeding.

1. Fork this repository into your own repo.
2. Create a branch: `git checkout -b <branch_name>`.
3. Make changes and commit them: `git commit -m '<commit_message>'`
4. Push to the original branch: `git push origin TrueBlocks/trueblocks-core`
5. Create the pull request.

## Contributors
---
Thanks to the following people who have contributed to this project:

* [@tjayrush](https://github.com/tjayrush)
* [@crodnun](https://github.com/crodnun)
* [@wildmolasses](https://github.com/wildmolasses)

## Contact
---
If you have specific requests, contact us here <info@quickblocks.io>.

## License
---
This project licensed under the [Apache License Version 2.0](LICENSE.md).
