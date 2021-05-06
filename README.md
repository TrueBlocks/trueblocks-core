# TrueBlocks Core

![GitHub repo size](https://img.shields.io/github/repo-size/scottydocs/README-template.md)
![GitHub contributors](https://img.shields.io/github/contributors/scottydocs/README-template.md)
![GitHub stars](https://img.shields.io/github/stars/TrueBlocks/trueblocks-core?style%3Dsocial)
![GitHub forks](https://img.shields.io/github/forks/TrueBlocks/trueblocks-core?style=social)
![Twitter Follow](https://img.shields.io/twitter/follow/trueblocks?style=social)

TrueBlocks is a collection of libraries, tools, and applications that improve access to the Ethereum data while remaining fully local. The interface is similar to the Ethereum RPC, but offers a number of improvements:

1) TrueBlocks allows you to scrape the chain to build an index of address appearances. This index enables lightning-fast access to transactional histories for a given address (something not available from the node itself),

2) TrueBlocks also provides for a local binary cache of data extracted from the node. This speeds up subsequent queries for the same data by an order of magintude or more. This enables a much better user experience for distributed applications written directly against the node such as the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer),

3) TrueBlocks enhances the Ethereum RPC interfaces. For example, you may query blocks and transactions by date, by block range, by hashes or any combination. Furthermore, two additional endpoints are provided for extracting (`export`) and listing (`list`) historical transactions per address.

## Prerequisites

---

**Golang**:

For all operating systems, please follow [these instructions](https://golang.org/doc/install) to install `golang` on your system.

**For Linux**:

```shell
sudo apt install build-essential git cmake python python-dev libcurl3-dev clang-format jq
```

**For Mac**:

```shell
brew install cmake
brew install git
brew install clang-format
brew install jq
```

We recommend that you run MacOS Big Sur or later for best results.

**For Windows**:

TrueBlocks does not currently support Windows builds.

## Compiling TrueBlocks

```shell
git clone -b develop git@github.com:TrueBlocks/trueblocks-core.git
cd trueblocks-core
mkdir build && cd build
cmake ../src
make
```

(You may use `make -j <ncores>` to parallelize the build. Replace `<ncores>` with the number of cores on your machine.)

The compilation creates a number of executables in `../bin`.

Run this command

```shell
../bin/chifra --version
```

You should get a version string similar to the below

```shell
trueBlocks GHC-TrueBlocks//0.9.0-alpha-409aa9388-20210503
```

If not, review the above commands and make sure you didn't miss something. [Create an issue](https://github.com/TrueBlocks/trueblocks-core/issues) if you continue to have trouble.

## Adding ./bin to your $PATH

`chifra` will only work if its underlying tools are found in your $PATH.

Add the full path to `./trueblocks-core/bin` to your shell's default environment. To find the full path, do this

```shell
cd ../bin && pwd && cd -
```

Add the result of that command to your shell's $PATH. If you don't know what we mean, a Google search may be in order...

## Introducing chifra

Similar to `git`, TrueBlocks has an overarching command called `chifra` that gives you access to all of the other subcommands.

Type:

```shell
chifra
```

You will see a long list of commands.

You can get more help on any command with `chifra <cmd> --help`.

### Getting status

Let's look at the first subcommand, called `status`.

```shell
chifra status --terse
```

If you get a bunch of JSON data, congratulations, your installation is working. You may skip ahead to the 'Using TrueBlocks' section below.

### -- Troubleshooting

Depending on your setup, you may get the following error message when you run some `chifra` commands:

```shell
  Warning: A request to your Ethereum node (http://localhost:8545) resulted
  in the following error [Could not connect to server]. Specify a valid
  rpcProvider by editing $CONFIG/trueblocks.toml.
```

If you get this error, edit the configuration file mentioned. The file is well documented, so refer to that file for further information.

When the `chifra status` command returns a valid response, you may move to the next section. If
you continue to have trouble, join our [discord disscussion](https://discord.gg/kAFcZH2x7K).

## Using chifra

If you've gotten this far, you're ready to use TrueBlocks. Run this command which shows every 10th block between the first and the 100,000th.

```shell
chifra blocks 0-100000:10
```

Hit `Control+C` to stop the processing.

## Building the TrueBlocks Index of Appearances

---

The primary data structure produced by TrueBlocks is an index of address appearances. This index provides very quick access to transaction histories for a given address.

You may either build the entire index from scratch (requires a tracing, archive node), or you may download part of the index and build it from there on.

This process is described in this article [Indexing Addresses](/docs/prologue/indexing/).

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
