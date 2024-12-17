---
title: "Running Against Multiple Rpc Endpoints"
description: "Running Against Multiple Rpc Endpoints"
lead: "Can I run against different RPC endpoints?"
categories:
- Tutorials
tags: ['tutorials']
date: 2021-04-21T17:13:04-04:00
lastmod: 2021-04-21T17:13:04-04:00
draft: false
images: ["running-against-mutliple-rpc-endpoints.jpg"]
contributors: ["Thomas Jay Rush"]
weight: 953
---
 
One user asked if it was possible to run TrueBlocks against multiple RPC endpoints at the same time. This is short recipe to accomplish just that.

## Preliminaries

The first thing to understand is that the TrueBlocks core runs against a local configuration file stored in your $HOME folder called `$HOME/.local/share/TrueBlocks/trueBlocks.toml`. It's an almost ridiculously simple file.

The file gets created and populated with default values the first time you build TrueBlocks. When you do `cmake ../src` and then `make` in the build folder of the TrueBlocks repo, you can look inside the file `$HOME/.local/share/trueblocks/trueBlocks.toml`.

You will see something like this:

```[toml]
[settings]
rpcProvider = "http://localhost:8545"
cachePath = "~/.local/share/trueblocks/trueBlocks/cache/mainnet/"
```

or something similar. This is a standard TOML with a heading or group for each set of unique keys and key / value pairs defining the options. While each tool and application in TrueBlocks has its own configuration file, the main options are stored in the `trueBlocks.toml` file. See the documentation for more information.

## Customizing the RPC Endpoint

Unless you're running your own node, you will have to customize the value of the `rpcProvider` variable. For example, if you use Infura as your node provider, you will get an Infura endpoint along with your Infura API key. You may enter that value in this file to give TrueBlocks direction in where to look for the RPC data.

We recommend you put the value in quotes, although this is optional.

In general operation, you can modify the `rpcProvider` value once and from then on, every time TrueBlocks interacts with the node, it will use that value.

In the case of our user, he/she wished to run two copies of TrueBlocks against two different RPC endpoints. What do to?

## Running Against Different RPC Endpoints

Every setting in any of the TrueBlocks configuration files is a key / value pair that lives under a group header (as is standard TOML). In the case of the `rpcProvider` the key is `rpcProvider` and the heading is `settings`.

Whenever you're running any of the TrueBlocks tools or applications, if you're starting them from the command line, you can customize a setting by exporting an environment variable prior to running the command.

An easy way to demostrate this is with the `chifra status` command.

Open a terminal window and navigate to the TrueBlocks `./build` folder that you created when first installing and building TrueBlocks. Run a test command just to prove you're working with a valid RPC provider already:

```[bash]
chifra blocks 12
```

This should return valid JSON. If not, go back to the initial installation and build instructions and make sure you're able to see valid data from the node.

Assuming you have a valid RPC endpoint, do this from the same folder:

```[bash]
> chifra status | head
```

(We pipe through `head` to show the only the first few lines.) You should get a response similar to this one:

```[json]
{
  "client_version": "OpenEthereum//v3.2.3-stable-a716eb3-20210415/x86_64-linux-gnu/rustc1.51.0",
  "client_ids": "chainId: 1 networkId: 1",
  "trueblocks_version": "GHC-TrueBlocks//0.8.5-alpha-f795c80d9-20210421",
  "rpc_provider": "http://localhost:36963",
  ....
```

Now do this (this works for `bash`, adjust for your own shell. We assume your providing a valid RPC endpoint):

```[bash]
> export SETTINGS_RPCPROVIDER="http://localhost:23456"
> chifra status | head
```

```[json]
{
  "client_version": "OpenEthereum//v3.2.3-stable-a716eb3-20210415/x86_64-linux-gnu/rustc1.51.0",
  "client_ids": "chainId: 1 networkId: 1",
  "trueblocks_version": "GHC-TrueBlocks//0.8.5-alpha-f795c80d9-20210421",
  "rpc_provider": "http://localhost:23456",
  ....
```

You should get a different results.

## Upshot

Any configuration value for any tool or application may be customized in the environment by specify a new value using the item's group and key, thus:

```[bash]
export GROUP_KEY = <new value>
```

In our case, group was 'settings', key was 'rpcProvider', so the environment variable was 'SETTINGS_RPCPROVIDER'.

Simple enough. Recipe done.

Cheers.
