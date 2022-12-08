<!-- markdownlint-disable MD033 MD036 MD041 -->
<b>If you have an existing installation, complete applicable [Migrations](https://github.com/TrueBlocks/trueblocks-core/blob/develop/MIGRATIONS.md) before proceeding.</b><hr>

<font size="+2">TrueBlocks / Unchained Index</font>

![GitHub repo size](https://img.shields.io/github/repo-size/TrueBlocks/trueblocks-core)
[![GitHub contributors](https://img.shields.io/github/contributors/TrueBlocks/trueblocks-core)](https://github.com/TrueBlocks/trueblocks-core/contributors)
[![GitHub stars](https://img.shields.io/github/stars/TrueBlocks/trueblocks-core?style%3Dsocial)](https://github.com/TrueBlocks/trueblocks-core/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/TrueBlocks/trueblocks-core?style=social)](https://github.com/TrueBlocks/trueblocks-core/network/members)
[![Twitter Follow](https://img.shields.io/twitter/follow/trueblocks?style=social)](https://twitter.com/trueblocks)

**Table of Contents**

- [Introduction](#introduction)
- [Installing](#installing)
- [Using chifra](#using-chifra)
- [Troubleshooting](#troubleshooting)
- [The unchained index](#the-unchained-index)
- [Docker version](#docker-version)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [Contact](#contact)
- [List of Contributors](#list-of-contributors)

## Introduction

TrueBlocks improves access to blockchain data for any EVM-compatible chain (particularly Ethereum mainnet) while remaining entirely local.

Features include:

- `chifra init` and `chifra scrape` which builds the Unchained Index, an index of address appearances that provides lightning-fast access to transactional histories,

- A built-in binary cache, which speeds up second and subsequent queries by orders of magnitude,

- Many enhanced command-line options enabling a new level of access to the data. Perfect for data scientists. Extract all `logs` produced by a contract, view all ERC-20 tokens from or to your account, etc.,

- Advanced tools that can produce reconciled "bank statements" and accounting for any asset on any address.

## Installing

These instructions assume you know how to use the command line. If you need help, see the [installation's troubleshooting section](https://trueblocks.io/docs/install/install-trueblocks/#troubleshooting) and or consult Google.

TrueBlocks runs on Linux and Mac. There is no support, nor will there ever be, for Windows.

**Install dependencies**

Install the [latest Go version](https://golang.org/doc/install). Verify that you are running at least version 1.18 of the Go tools:

```[shell]
go version
```

Install the following build dependencies for your operating system:

`build-essential git cmake ninja python3 libcurl3-dev clang-format jq`

Consult Google for particulars for your operating system (probably `apt-get` on linux and `brew install` on Mac).

**Download and build**

```[shell]
git clone -b develop https://github.com/trueblocks/trueblocks-core
cd trueblocks-core
mkdir build && cd build
cmake ../src
make
```

**Note:** You may speed up the build with `make -j <ncores>` where `ncores` represents the number of cores to use.

**Add `./trueblocks-core/bin` to your $PATH**

Search for instructions for setting your shell's $PATH on Google. The following assumes you've set the $PATH properly so that `chifra --help` returns results.

**Edit the configuration**

If you've sucessfully built the executable and set the `$PATH`, you should be able to run:

```[shell]
chifra status --terse
```

and get valid results. If you get an error, the most likely cause is a misconfiguration. The error message will report the location of the config file:

- `~/.local/share/trueblocks/trueBlocks.toml`, or
- `~/Library/Application Support/TrueBlocks/trueBlocks.toml`, or
- `$XDG_CONFIG_HOME` if set.

Edit the `trueblocks.toml` file. Most likely, you need to set the `rpcProvider` to point to a valid RPC endpoint for your chain.

The configuration file is reasonably-well documented. It should be self-explanitory.

**Searching account histories**

Most likely, you will want to use the Unchained Index. To do so, follow these instructions: [get the index](https://trueblocks.io/docs/install/get-the-index/)

To use the Account Explorer (which is still pre-beta), see [installing the explorer](https://trueblocks.io/docs/install/install-explorer/)

## Using chifra

The TrueBlocks command-line tool is called `chifra`. This gives you access to all the other tools:

```[shell]
chifra --help
```

Get more help on any sub-command with `chifra <cmd> --help`. Full documentation is available on [our website](https://trueblocks.io).

**Getting status**

Let's look at the first subcommand, called `status`.

```[shell]
chifra status --terse
```

If you get a bunch of JSON data, congratulations, your installation is working.  Try this command which shows every 10th block between the first and the 100,000th:

```[shell]
chifra blocks 0-100000:10
```

You should see a long stream of data. Kill the display with `Control+C`.

See the entire list of chifra commands with `chifra --help`.

## Troubleshooting

Depending on your setup, you may get the following error message when you run some `chifra` commands:

```[shell]
  Warning: A request to your Ethereum node (http://localhost:8545)
  resulted in the following error [Could not connect to server].
  Specify a valid rpcProvider by editing $CONFIG/trueblocks.toml.
```

Edit the file as instructed. You may find helpful [answers on our FAQ](https://trueblocks.io/blog/faq/).

See [our blog](https://trueblocks.io/blog/) for a lot of usful articles on getting started and using TrueBlocks.

If you continue to have trouble, join our [discord discussion](https://discord.gg/kAFcZH2x7K)

## The unchained index

The primary data structure produced by TrueBlocks is an index of address appearances called the Unchained Index. This index provides very quick access to transaction histories for a any address.

You may either build the entire index from scratch (requires an EVM-compatible tracing/archive node) or you may download a snapshot of the index build from there.

This process is described in the article [Indexing Addresses](https://trueblocks.io/docs/install/get-the-index/).

## Docker version

Our [official docker version](https://github.com/TrueBlocks/trueblocks-docker) is in a separate repo. Please see that repo for more information on running with Docker.

## Documentation

The TrueBlocks [documentation repo](https://github.com/TrueBlocks/trueblocks-docs) builds the TrueBlocks website. See our website for the [best available documentation](https://trueblocks.io/).

## Contributing

A chart showing the number of stars on our repo over time.

[![Stargazers over time](https://starchart.cc/TrueBlocks/trueblocks-core.svg)](https://starchart.cc/TrueBlocks/trueblocks-core)

We love contributors. Please see information about our [work flow](https://github.com/TrueBlocks/trueblocks-core/blob/develop/docs/BRANCHING.md) before proceeding.

1. Fork this repository into your own repo.
2. Create a branch: `git checkout -b <branch_name>`.
3. Make changes to your local branch and commit them to your forked repo: `git commit -m '<commit_message>'`
4. Push back to the original branch: `git push origin TrueBlocks/trueblocks-core`
5. Create the pull request.

## Contact

If you have questions, comments, or complaints, please join the discussion on our discord server which is [linked from our website](https://trueblocks.io).

## List of Contributors

Thanks to the following people who have contributed to this project:

- [@tjayrush](https://github.com/tjayrush)
- [@dszlachta](https://github.com/dszlachta)
- [@wildmolasses](https://github.com/wildmolasses)
- [@MysticRyuujin](https://github.com/MysticRyuujin)
- [@MattDodsonEnglish](https://github.com/MattDodsonEnglish)
- [@crodnun](https://github.com/crodnun)

