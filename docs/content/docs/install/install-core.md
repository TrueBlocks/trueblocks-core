---
title: "Install core"
description: "TrueBlocks works on Linux and Mac"
lead: "TrueBlocks runs on Linux and Mac. We do not support Windows."
date: 2020-11-16T13:59:39+01:00
lastmod: 2020-11-16T13:59:39+01:00
draft: false
images: []
aliases:
  - "/docs/prologue/installing-trueblocks"
  - "/docs/install/install-trueblocks"
menu:
  docs:
    parent: "install"
weight: 0200
toc: true
---

<!-- markdownlint-disable MD041 MD033 -->

These instructions assume you can navigate the command line and edit configuration files.
If you need help with a particular step, see the [installation's troubleshooting section](/docs/install/install-troubleshooting).

## Installation

- Open a terminal window.

- Type `go version`. If Go is not installed or less than Go Version 1.23, [install the latest version of Go](https://go.dev/doc/install)

- Install or upgrade the build dependencies: _git_, _cmake_, _ninja_, _curl_, _python_, _clang-format_, _jq_

  - [Install dependencies](https://trueblocks.io/docs/install/install-troubleshooting/#installing-build-tools)
<br><br>
- Clone the repo and compile the executable:

```shell
git clone --depth 1 --no-single-branch --recurse-submodules --branch develop https://github.com/TrueBlocks/trueblocks-core
cd trueblocks-core
mkdir build && cd build
../scripts/go-work-sync.sh
cmake ../src
make
```

- Switch to the `master` branch if you want a more stable version.

- The build may be faster with `make -j <nproc>`.

- Add `./trueblocks-core/bin` to your shell's $PATH. Check Google if you don't know what this means.

- Type `chifra config --paths`. This will produce a display similar to the following (ignore any errors). Note the `$CONFIG` paths.

```[shell]
chifra config --paths:
  Config: $HOME/Library/Application Support/TrueBlocks/
  Cache : $HOME/Library/Application Support/TrueBlocks/cache/mainnet
  Index : $HOME/Library/Application Support/TrueBlocks/unchained/mainnet
```

- Next, edit a configuration file called `$CONFIG/trueBlocks.toml`. Edit this file and locate the `[chains.mainnet]` section. Add a valid RPC endpoint. If you don't know what this means, search Google.

```[shell]
[chains.mainnet]
...
rpcProvider = "http://localhost:8545"
...
```

- If you wish to use the `--articulate` feature (you probably do), add an API key to the following section:

```[shell]
[keys.etherscan]
...
apiKey = "<your Etherscan api key>"   # optional
...
```

## Testing the install

If you've installed things properly, you should be able to now run the following command, which should return this data:

```[shell]
chifra blocks 12
```

You should get results similar to these:

```[shell]
{
  "data": [
    {
      "gasLimit": 5000,
      "gasUsed": 0,
      "hash": "0xc63f666315fa1eae17e354fab532aeeecf549be93e358737d0648f50d57083a0",
      "blockNumber": 12,
      "parentHash": "0x3f5e756c3efcb93099361b7ddd0dabfeaa592439437c1c836e443ccb81e93242",
      "miner": "0x0193d941b50d91be6567c7ee1c0fe7af498b4137",
      "difficulty": 17179844608,
      "finalized": true,
      "timestamp": 1438270144,
      "baseFeePerGas": 0
    }
  ]
}
```

- Assuming the above works, your system is properly installed. If not, see the [TroubleShooting section](/docs/install/install-troubleshooting).

## More information

There's a lot left to learn. Have fun:

- [Using the Unchained Index](/docs/install/build-unchained-index/)
- [Explore some coding examples](https://github.com/TrueBlocks/trueblocks-examples)
- [View data science recipes](/tags/tutorials/)
- [Use the explorer application](/docs/install/install-explorer/)
