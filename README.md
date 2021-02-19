# TrueBlocks Core

Command line application that lets you make simple or complex queries of ethereum
data. Works how an ethereum application should:
**local-first**, **lightning fast**, and **private by default**.

![Twitter Follow](https://img.shields.io/twitter/follow/trueblocks?style=social)

─[What it does](#what-it-does)─[Build](#Building-trublocks)─[Test](#Test-install)─[Make infinite queries](#make-infinite-queries)─[Contribute](#contribute)─[Get in touch](#contact)─

![A gif showing quotes](./docs/quotes.gif)

## What it does

With TrueBlocks, you have an index of every appearance of every address on the chain.
Because it uses a  binary cache, this application is fast. Because
the cache consists of only the data you extracts, this application is minimal. All of this means a huge amount of ethereum data at your disposal, with an app that is:

* Fully decentralized, private by default.
* Very fast
* Minimal. Only the data you need, very few dependencies.

If you want a GUI, TrueBlock-core is also the engine of our [Explorer](http://github.com/TrueBlocks/trueblocks-explorer) application.

## Build

All you need is an ethereum node and a few build tools. Then compile the program and start pulling data.

### Prereqs

The application has minimal dependancies: only `go`, `git`, and some basic build tools like
`cmake` and `clang-format`. To install `golang` on your system, follow [these instructions](https://golang.org/doc/install).

To install the build tools, run these commands:

* On Linux

```[shell]
sudo apt install build-essential git cmake python python-dev libcurl3-dev clang-format jq
```

* On Mac

```[shell]
brew install cmake
brew install git
brew install clang-format
brew install jq
```

#### An Ethereum node

Of course, you'll also need an Ethereum node. This node can be running locally on your machine, or remotely
on a service like [Infura](https://infura.io/dashboard).

### Building Trueblocks

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

## Test install

After building TrueBlocks, test your configuration with this command:

```[shell]
chifra --version
```

Next, check that you can get a block from your Ethereum node. Lets check out block 100:

```[shell]
chifra blocks 100
```

If you're running your node locally, this should return valid JSON data for block 100. 
If you want data from an external RPC provider, you've probably got an error like this:

```[shell]
Warning: The Ethereum RPC: 'http://localhost:8545' was not found. Quitting...
```

In that case, you need to add a provider's URL. In the file `~/.quickBlocks/quickBlocks.toml`, add this text:

```[toml]
[settings]
rpcProvider="<url-to-your-rpc-provider>
```

Once you get `chifra blocks` to return JSON data, you are ready to test your
installation. In the `./build` directory, run this command:

```[shell]
make tests
```

### ethslurp requires an API key

One optional tool, `ethslurp`, requires an EtherScan key. To use `ethslurp`, get an EtherScan API key, then add the following to the [settings] group of
`~/.quickBlocks/quickBlocks.toml`:

```toml

[settings]
etherscan_key = "{YOUR_KEY}"
```

## Make infinite queries

Again, if you want a graphic display, checkout our [Explorer app](http://github.com/TrueBlocks/trueblocks-explorer).
Otherwise, let's see what we can do on the CLI.

TrueBlocks ships with huge amount of tools that can pull all kinds of ethereum data─
blocks, transactions, receipts, logs, traces, names, tokens, state, ABIs, etc.

Luckily, all of these commands are organized under one master command, `chifra`.
Meaning "cipher" in arabic, `chifra` is the one command to rule them all.

To check the full list of commands, run:

```shell
chifra --help --verbose
```

Lets say you want a quick pull of the DAI stablecoin ERC20. `chifra names` helps
you there.

```shell
chifra names Dai Stablecoin ERC20
```

Let's do something more interesting. Say we want to get all the transactions
for the Wrapped Ether address. With `chifra slurp`, we can query the address.

```shell
chifra slurp 0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2
```

That's a lot of output. Maybe you it'd be easier to handle as a CSV:

```shell
chifra slurp --fmt:csv 0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2 > weth.csv
```

Now you have a ten-thousand line file for your spreadsheet software.

These two examples don't even scratch the surface! To figure out how to make the query you
want, check out [the full list of chifra commands and options](./docs/chifra.md).

## Contribute

**Issues**: If you find any bugs, have any feature requests, or just feel confused, feel free to make an issue. Don't be shy! Each issue helps us make TrueBlocks better.

**Contributions**: From big optimizations to typo fixes, we welcome any help we can get. If you see how you can fix something, you can fork and make a pull request. We appreciate each one.

## Contact

Come and chat on [our discord](https://discord.com/invite/c6KDJXvX)!
