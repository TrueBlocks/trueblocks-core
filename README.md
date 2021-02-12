# TrueBlocks Core

![GitHub repo size](https://img.shields.io/github/repo-size/scottydocs/README-template.md)
![GitHub contributors](https://img.shields.io/github/contributors/scottydocs/README-template.md)
![GitHub stars](https://img.shields.io/github/stars/Great-Hill-Corporation/trueblocks-core?style%3Dsocial)
![GitHub forks](https://img.shields.io/github/forks/Great-Hill-Corporation/trueblocks-core?style=social)
![Twitter Follow](https://img.shields.io/twitter/follow/trueblocks?style=social)

Build local-first, fully-decentralized applications using data directly from
an Ethereum node. TrueBlocks works through two mechanisms:

* A lightning-fast index of every appearance of every addresses on the chain, and
* A binary cache of only the data your application extracts.

Local-first means your application is **private by default**. The client-side cache means your application is **fast**.  *Extraction-on-demand* means your application remains **minimal**.

[How does TrueBlocks Work?](./docs/FAQ.md#how-it-works)

## Prerequisites

Before building TrueBlocks, you need to have `git`, `cmake`, `clang-format`, and `go` available.

To install `golang` on your system, follow [these instructions](https://golang.org/doc/install).

To install `git`, `cmake`, and `clang-format`, run these commands.

### On Linux

```[shell]
sudo apt install build-essential git cmake python python-dev libcurl3-dev clang-format jq
```

### On Mac

```[shell]
brew install cmake
brew install git
brew install clang-format
brew install jq
```

### An Ethereum node

Besides the build tools, the only thing TrueBlocks requires is access to an
Ethereum node. This node code be running locally on your machine, or remotely
on a service like [Infura](https://infura.io/dashboard).

## Building TrueBlocks

Currently, you must build TrueBlocks from source:

```[shell]
git clone git@github.com:TrueBlocks/trueblocks-core.git
cd trueblocks-core
mkdir build && cd build
cmake ../src
make
```

This creates a series of executables in the `./bin` folder at the top of the
repo. If you want to use the command line tools, export this folder to your `$PATH`.

[Does TrueBlocks Work on Windows?](./docs/FAQ.md-windows)

## Testing Installation

After building TrueBlocks, you may test your configuration with this command:

```[shell]
chifra --version
```

For a complete list of available commands, run this command:

```[shell]
chifra --help --verbose
```

Next, check that you can get a block from your Ethereum node. Enter this command:

```[shell]
getBlock 100
```

This should return valid JSON data for block 100 (type `getBlock --help` for
more options with this tool). If you get an error like this:

```[shell]
Warning: The Ethereum RPC: 'http://localhost:8545' was not found. Quitting...
```

You need to edit the file `~/.quickBlocks/quickBlocks.toml` to provide the URL of an available Ethereum RPC provider. Add this text:

```[toml]
[settings]
rpcProvider="<url-to-your-rpc-provider>
```

Once you get `getBlock` to return JSON data, you are ready to test your installation. In the `./build` directory, run this command:

```[shell]
make tests
```

### ethslurp requires an API key

One optional tool, `ethslurp`, requires an EtherScan key. To use `ethslurp`, get an EtherScan API key, then add the following to `~/.quickBlocks/quickBlocks.toml`:

```toml
etherscan_key = "{YOUR_KEY}"
```

## Using TrueBlocks
---

The `chifra` tool is the basis for everything related to TrueBlocks. (`chifra` is derived from the Arabic word for `cipher`.) It helps you decipher the chain data.

[Running the API](./docs/FAQ.md#running-the-api)

[Using the Command Line](./docs/FAQ.md#using-the-command-line)

[Using the Libraries](./docs/FAQ.md#using-the-library)

[Where to Go from Here...](./docs/FAQ.md#where-to-go-from-here)

[Full Documentation...](./docs/FAQ.md#full-documentation)

## Contributing to TrueBlocks

Please see information about our [work flow](./docs/BRANCHING.md) before proceeding.

1. Fork this repository.
2. Create a branch: `git checkout -b <branch_name>`.
3. Make changes and commit them: `git commit -m '<commit_message>'`
4. Push to the original branch: `git push origin TrueBlocks/trueblocks-core`
5. Create the pull request.

## Contributors

Thanks to the following people who have contributed to this project:

* [@tjayrush](https://github.com/tjayrush)
* [@crodnun](https://github.com/crodnun)
* [@wildmolasses](https://github.com/wildmolasses)

## Contact

If you have specific requests, contact us here <info@quickblocks.io>.

## License

This project licensed under the (GNU General Public License v3.0)(LICENSE.md).
