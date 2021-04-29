# TrueBlocks Core

![GitHub repo size](https://img.shields.io/github/repo-size/scottydocs/README-template.md)
![GitHub contributors](https://img.shields.io/github/contributors/scottydocs/README-template.md)
![GitHub stars](https://img.shields.io/github/stars/TrueBlocks/trueblocks-core?style%3Dsocial)
![GitHub forks](https://img.shields.io/github/forks/TrueBlocks/trueblocks-core?style=social)
![Twitter Follow](https://img.shields.io/twitter/follow/trueblocks?style=social)

TrueBlocks allows you to build local-first, fully-decentralized applications using data directly from an Ethereum node. It does this through two mechanisms:

1. A lightning-fast index of every appearance of every addresses on the chain, and

2. A binary cache of only the data your application extracts.

Local-first means your application is **private by default**, the client-side cache means your application is **fast**, and *extraction-on-demand* means your application will remain **minimal**.

[How does TrueBlocks Work?](./docs/FAQ.md#how-it-works)

## Prerequisites

---

Before building TrueBlocks, make sure you have `git`, `cmake`, `clang-format`, and `go` available.

Follow [these instructions](https://golang.org/doc/install) for installing `golang` on your system.

Complete these commands to install `git`, `cmake`, and `clang-format`:

### On Linux:

```[shell]
sudo apt install build-essential git cmake python python-dev libcurl3-dev clang-format jq
```

### On Mac:

```[shell]
brew install cmake
brew install git
brew install clang-format
brew install jq
```

### On Windows:

We currently have no plans to support Windows builds. You may use an emulated linux system on Windows to accomplish the build. [Does TrueBlocks Work on Windows?](./docs/FAQ.md-windows)


## Building TrueBlocks

---

The only way to install TrueBlocks is to build it yourself. Follow the pre-requisites above, and then follow these instructions:

```[shell]
git clone git@github.com:TrueBlocks/trueblocks-core.git
cd trueblocks-core
mkdir build && cd build
cmake ../src
make
```

This will create executables in the `./bin` folder at the top of the repo. Please make sure to add that path to your environment's `$PATH`.

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

Next, let's see if you can use TrueBlocks to extract a block from your Ethereum node. Enter this command:

```[shell]
chifra blocks 1000
```

This should return valid JSON data for block 1,000 (type `getBlock --help` for more options on this tool.) If you get an error, such as:

```[shell]
Warning: The Ethereum RPC: 'http://localhost:8545' was not found. Quitting...
```

To fix this problem, first identify the location of your configuration files. On Mac, your configuration files are at `~/Library/Application Support/TrueBlocks`. On Linux, your configuration files are at `~/.local/share/trueblocks`. We call the configruation folder $CONFIG below.

To fix the missing RPC Provider issue above, edit `$CONFIG/trueBlocks.toml` and insert a valid provider. You can get a provider from various places including Infura, however, we think running your own node is best.

Enter the following text to specify a provider (add the section header if it doesn't already exist):

```[toml]
[settings]
rpcProvider="<url-to-your-rpc-provider>
```

Returning to your terminal, this command should now produce data:

```[shell]
chifra blocks 1000
```

You may test more thouroughly by running the following command:

```[shell]
make tests
```

You may find that the above command show many broken tests. While not required, one of the tools (`ethslurp`) requires an API key from Etherscan. Get an EtherScan API key and add it to the same file above under the `[settings]` section thus:

```[toml]
...
etherscan_key = "<your key>"
```

## Using TrueBlocks

---

`chifra` is the basis for everything related to TrueBlocks. (The word `chifra` is derived from the Arabic word for `chipher`.) Chifra helps you decipher chain data as well as manage various aspects of the system.

For further information, please see these articles:

[Running the API](./docs/FAQ.md#running-the-api)

[Using the Command Line](./docs/FAQ.md#using-the-command-line)

[Using the Libraries](./docs/FAQ.md#using-the-library)

[Where to Go from Here...](./docs/FAQ.md#where-to-go-from-here)

[Full Documentation...](./docs/FAQ.md#full-documentation)

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

## License

---
Parts of this project licensed under the [Apache License Version 3.0](LICENSE.md).
