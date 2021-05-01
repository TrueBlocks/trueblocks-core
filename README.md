# TrueBlocks Core

Ethereum as it was meant to be: local-first, P2P, and accurate queries to all the data you’ll ever need.

## Installing

1. A lightning-fast index of every appearance of every addresses on the chain, and

2. A binary cache of only the data your application extracts.

This is a work in progress. See [What works now?](#_what_works_now) for known issues.

</div>

## Prerequisites

---

Before building TrueBlocks, make sure you have `git`, `cmake`, `clang-format`, and `go` available.

    **For Linux**:

        sudo apt install build-essential git cmake python python-dev libcurl3-dev clang-format jq

### On Linux:

We recommend that you run MacOS Big Sur or later for best results.

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

        git clone -b develop git@github.com:TrueBlocks/trueblocks-core.git
        cd trueblocks-core
        mkdir build && cd build
        cmake ../src
        make -j

This will create executables in the `./bin` folder at the top of the repo. Please make sure to add that path to your environment's `$PATH`.

## Testing Installation

---

4.  Test your install.

        chifra blocks 14560

## What even is this?

TrueBlocks creates an index that lets you access the entire Ethereum chain directly from your local machine. It ships with a large number of tools that let you chain together queries on all types of Ethereum data: transaction numbers, addresses, blocks, ABIs, et cetera.

Next, let's see if you can use TrueBlocks to extract a block from your Ethereum node. Enter this command:

Through a binary cache, TrueBlocks can make your repeated queries *very fast*. For large complex queries, this cache can reduce query times from minutes to seconds.

This should return valid JSON data for block 1,000 (type `getBlocks --help` for more options on this tool.) If you get an error, such as:

-   Data scientists

To fix this problem, first identify the location of your configuration files. On Mac, your configuration files are at `~/Library/Application Support/TrueBlocks`. On Linux, your configuration files are at `~/.local/share/trueblocks`. We call the configruation folder $CONFIG below.

To fix the missing RPC Provider issue above, edit `$CONFIG/trueBlocks.toml` and insert a valid provider. You can get a provider from various places including Infura, however, we think running your own node is best.

Enter the following text to specify a provider (add the section header if it doesn't already exist):

-   Organizations like charities who may need to process restricted donations.

Returning to your terminal, this command should now produce data:

```[shell]
chifra blocks 1000
```

You may test more thouroughly by running the following command:

### The catch

You may find that the above command show many broken tests. While not required, one of the tools (`ethslurp`) requires an API key from Etherscan. Get an EtherScan API key and add it to the same file above under the `[settings]` section thus:

```[toml]
...
etherscan_key = "<your key>"
```

## Using TrueBlocks

---

`chifra` is the basis for everything related to TrueBlocks. (The word `chifra` is derived from the Arabic word for `chipher`.) Chifra helps you decipher chain data as well as manage various aspects of the system.

For further information, please see these articles:

Back in late 2015, we become obsessed with Ethereum. Not because of 'price go up', but because of the amazing promise that shared, global data has to change the world:

-   Permissionless data

-   World-wide consented-to data

-   Cryptographically secured data

-   Distributed data

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
