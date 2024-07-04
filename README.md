<b>If you have an existing installation, complete applicable [Migrations](https://github.com/TrueBlocks/trueblocks-core/blob/develop/MIGRATIONS.md) before proceeding.</b><hr>

<h1>TrueBlocks / Unchained Index</h1>

![GitHub repo size](https://img.shields.io/github/repo-size/TrueBlocks/trueblocks-core)
[![GitHub contributors](https://img.shields.io/github/contributors/TrueBlocks/trueblocks-core)](https://github.com/TrueBlocks/trueblocks-core/contributors)
[![GitHub stars](https://img.shields.io/github/stars/TrueBlocks/trueblocks-core?style%3Dsocial)](https://github.com/TrueBlocks/trueblocks-core/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/TrueBlocks/trueblocks-core?style=social)](https://github.com/TrueBlocks/trueblocks-core/network/members)
[![Twitter Follow](https://img.shields.io/twitter/follow/trueblocks?style=social)](https://twitter.com/trueblocks)

**Table of Contents**

- [Introduction](#introduction)
- [Installing](#installing)
- [Command line](#command-line)
- [Chifra Serve](#chifra-serve)
- [Troubleshooting](#troubleshooting)
- [The unchained index](#the-unchained-index)
- [Docker version](#docker-version)
- [Documentation](#documentation)
- [Linting](#linting)
- [Contributing](#contributing)
- [Contact](#contact)
- [List of Contributors](#list-of-contributors)

## Introduction

TrueBlocks improves access to blockchain data for EVM-compatible chains (particularly Ethereum mainnet) while remaining entirely local.

Features:

- `chifra init` and `chifra scrape` which builds the Unchained Index, an index of address appearances that provides lightning-fast access to transactional histories,

- An optional binary cache, which speeds up queries to the RPC by orders of magnitude,

- Enhanced command-line options enabling much better access to chain data for data scientists and analysts. For example, easily extract all `logs` produced by a smart
contract or view all ERC-20 holdings for an account, etc.,

- Advanced tools for producing reconciled "bank statements" and accounting export for any token including ETH.

- An infinite number of other things restricted only by your imagination.

## Installing

Please see [the installation instructions](https://trueblocks.io/docs/install/install-trueblocks) on our website.

**Searching account histories**

While optional, you most likely want to use the Unchained Index to search account histories. To do so, [get the index](https://trueblocks.io/docs/install/get-the-index/).

**Account explorer**

You may use the command line, of course, to access data, but you may also wish to run an API server:

```[shell]
chifra daemon
```

Use `curl` to pull data or use it to drive our "pre-beta" Account Explorer. See [installing the explorer](https://trueblocks.io/docs/install/install-explorer/). The API provides the identical tools and options as the command line and it [documented here](https://trueblocks.io/api/).

## Command line

The TrueBlocks command-line tool is called `chifra`. This gives you access to all the other tools:

```[shell]
chifra --help
```

Get more help on any sub-command with `chifra <cmd> --help`. Full documentation is available on [our website](https://trueblocks.io).

## Chifra Serve

One of `chifra's` command line tools is called `serve`. This tool provides a simple API mimicing the command line. It is intended to be used only for single user enviroments.

To start the server, run:  

```[shell]
chifra server
```

<span style="size: -2; background-color: #febfc1; color: black; display: block; padding: 4px">Chifra was built for the command line, a fact we purposefully take advantage of to ensure continued operation on small machines. As such, this tool is not intended to serve multiple end users in a cloud-based server environment. This is by design. Be forewarned.</span>

Get more help on any sub-command with `chifra <cmd> --help`. Full documentation is available on [our website](https://trueblocks.io).

**Getting data**

Let's look at the first subcommand, called `status`.

```[shell]
chifra status
```

If you get a bunch of data, congratulations, your installation is working. Try this command which shows every 10th block between the first and the 100,000th:

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
  Specify a valid rpcProvider by editing $rootPath/trueblocks.toml.
```

Edit the file as instructed. You may find helpful [answers on our FAQ](https://trueblocks.io/blog/faq/).

See [our blog](https://trueblocks.io/blog/) for a lot of useful articles on getting started and using TrueBlocks.

If you continue to have trouble, join our [discord discussion](https://discord.gg/kAFcZH2x7K)

## The unchained index

The primary data structure produced by TrueBlocks is an index of address appearances called the Unchained Index. This index provides very quick access to transaction histories for any address.

You may either build the entire index from scratch (requires an EVM-compatible tracing/archive node) or you may download a snapshot of the index build from there.

This process is described in the article [Indexing Addresses](https://trueblocks.io/docs/install/get-the-index/).

## Docker version

Our [official docker version](https://github.com/TrueBlocks/trueblocks-docker) is in a separate repo. Please see that repo for more information on running with Docker.

## Documentation

The TrueBlocks [documentation repo](https://github.com/TrueBlocks/trueblocks-docs) builds the TrueBlocks website. See our website for the [best available documentation](https://trueblocks.io/).

## Linting

Our build process requires the code you submit to be linted.

In order to that, you must install the GoLang linters. [See this page for more information](https://golangci-lint.run/usage/install/).

To install the primary linter (called `golangci-lint`), run this command:

```[shell]
curl -sSfL https://raw.githubusercontent.com/golangci/golangci-lint/master/install.sh | sh -s -- -b $(go env GOPATH)/bin v1.53.3
```

Verify the installation with `golangci-lint --version`. You should see something like this:

```[shell]
golangci-lint has version 1.50.1 built from <commit> on <date>
```

Next, run `golangci-lint linters`. Your system should have [at least the default list](https://golangci-lint.run/usage/linters/) to properly lint your submission.

```[shell]
> golangci-lint linters


```
You are encouraged to use additional linters. If you do, and you think they're useful, please suggest that we add it to our build process.

## Contributing

A chart showing the number of stars on our repo over time.

[![Stargazers over time](https://starchart.cc/TrueBlocks/trueblocks-core.svg)](https://starchart.cc/TrueBlocks/trueblocks-core)

We love contributors. Please see information about our [workflow](https://github.com/TrueBlocks/trueblocks-core/blob/develop/docs/BRANCHING.md) before proceeding.

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
- many others
