# Recent Migrations

Please see this [Migration](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-0.14.0.md) which contains important information.

# TrueBlocks Core

![GitHub repo size](https://img.shields.io/github/repo-size/scottydocs/README-template.md)
![GitHub contributors](https://img.shields.io/github/contributors/scottydocs/README-template.md)
![GitHub stars](https://img.shields.io/github/stars/TrueBlocks/trueblocks-core?style%3Dsocial)
![GitHub forks](https://img.shields.io/github/forks/TrueBlocks/trueblocks-core?style=social)
![Twitter Follow](https://img.shields.io/twitter/follow/trueblocks?style=social)

## Table of Contents
  - [Introduction](#introduction)
  - [Quick Install](#quick-install)
  - [Introducing chifra](#introducing-chifra)
  - [Using chifra](#using-chifra)
  - [Building the Index of Appearances](#building-the-trueblocks-index-of-appearances)
  - [Contributing](#contributing-to-trueblocks)
  - [List of Contributors](#contributors)
  - [Contact](#contact)

## Introduction

TrueBlocks is a collection of libraries, tools, and applications that improve access to the Ethereum data while remaining fully local. The interface is similar to the Ethereum RPC, but offers a number of improvements:

1) TrueBlocks allows you to scrape the chain to build an index of address appearances. This index enables lightning-fast access to transactional histories for a given address (something not available from the node itself),

2) TrueBlocks also provides for a local binary cache of data extracted from the node. This speeds up subsequent queries for the same data by an order of magintude or more. This enables a much better user experience for distributed applications written directly against the node such as the [TrueBlocks Explorer](https://github.com/TrueBlocks/trueblocks-explorer),

3) TrueBlocks enhances the Ethereum RPC interfaces. For example, you may query blocks and transactions by date, by block range, by hashes or any combination. Furthermore, two additional endpoints are provided for extracting (`export`) and listing (`list`) historical transactions per address.

## Quick Install

TrueBlocks runs on Linux and Mac. There is no official Windows support.
Some users have had success using WSL─you're on your own!

These instructions assume you can navigate directories with the command line
and edit configuration files.
If you need help with a step, see the [installation's troubleshooting section](https://trueblocks.io/docs/install/install-trueblocks/#troubleshooting).

0. Install dependencies
    - &#9745; [Install the latest version of Go](https://golang.org/doc/install).
    - &#9745; Install the other dependencies with your command line: `build-essential` `git` `cmake` `python` `python-dev` `libcurl3-dev` `clang-format` `jq`.

1. Compile from the codebase
    ```shell
    git clone -b develop https://github.com/trueblocks/trueblocks-core
    cd trueblocks-core
    mkdir build && cd build
    cmake ../src
    make
    ```
    _(You may use `make -j <ncores>` to parallelize the build. <ncores> represents the number of cores to devote to the `make` job)_

2. Add `trueblocks-core/bin` to your shell `PATH`.

3. Find your TrueBlocks configuration directory. It should be in one of these places:

    * On linux at `~/.local/share/trueblocks`
    * On mac at `~/Library/Application Support/TrueBlocks`
    * If you've configured it, wherever the location of `$XDG_DATA_HOME` is

4. In the configuration directory, edit `trueblocks.toml` to add your RPC and API keys. It should look something like this:
```toml
[settings]
rpcProvider = "<url-to-rpc-endpoint>"
```

5. Test a command!
```shell
chifra blocks 12345
```
### Optional steps

6. To make deep data queries, [get the index](https://trueblocks.io/docs/install/get-the-index/)
7. To explore the data visually, [install the explorer application](https://trueblocks.io/docs/install/install-explorer/).

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
you continue to have trouble, join our [discord discussion](https://discord.gg/kAFcZH2x7K).

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

This process is described in this article [Indexing Addresses](https://trueblocks.io/docs/install/get-the-index/).

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
