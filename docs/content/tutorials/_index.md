---
title: "TrueBlocks in Berlin - 2023"
description: "Learn how to use TrueBlocks, chifra, and the Unchained Index."
date: 2023-09-05T19:25:12+02:00
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
layout: single
---

<!-- smarkdownlint-disable -->

An interactive demonstration of TrueBlocks at dAppCon Berlin 2023.

### Prior to attending the session

To save time for everyone, please complete the [Prerequisites](#prerequisites) before you come.

### What we hope to accomplish

```plaintext
"Accessing Ethereum data from a locally running Ethereum node
is orders of magnitude better than accessing it remotely."

Thomas Jay Rush, every day
```

We hope to show why:

- Running a local Ethereum node is not that hard,
- The RPC is broken, but it can be fixed,
- Web 2.0 APIs delivering Web 3.0 data makes NO SENSE!
  - Rate limiting slows the world to molasses,
  - Thier non-standard endpoints will capture you,
  - They MUST know who you are so they can rate limit you,
  - The API game is winner-take-all. Witness Google,
  - How do we know they're not lying?
  - Why wouldn't regulators shut them down?
- Content addressable data stores hold the key.

### Prerequisites

TrueBlocks is built from source and, therefore, you'll need a few build tools. Run the following commands depending on your operating system.

| Operating System | Command                                                                                                           |
| ---------------- | ----------------------------------------------------------------------------------------------------------------- |
| **Linux:**       | `sudo apt install build-essential git cmake ninja-build python3 python3-dev libcurl4-openssl-dev clang-format jq` |
| **Mac:**         | `brew install cmake ninja git clang-format jq`                                                                    |

You also need [GoLang at version 1.20.0](https://golang.org/doc/install) or later.

Check that things are working and install anything that's missing. Pay particular attention to the versions of the tools (especially golang). The build won't work if the versions are too old.

#### Required tools

| Tool   | run this...       | ...if you get this, you're done... | ...otherwise, download it                  |
| ------ | ----------------- | ---------------------------------- | ------------------------------------------ |
| git    | `git version`     | >= 2.17.1                          | [download](https://git-scm.com/downloads)  |
| cmake  | `cmake --version` | >= 3.20.1                          | [download](https://cmake.org/install/)     |
| golang | `go version`      | >= 1.20.0                          | [download](https://golang.org/doc/install) |

#### Optional tool

This tool is optional but is sometimes useful for making the data easier to see.

| Tool | run this...    | ...if you get this, you're done... | ...otherwise, download it                           |
| ---- | -------------- | ---------------------------------- | --------------------------------------------------- |
| jq   | `jq --version` | >= 1.6                             | [download](https://stedolan.github.io/jq/download/) |

### Building TrueBlocks

Use the following commands to build TrueBlocks...

```bash
git clone https://github.com/TrueBlocks/trueblocks-tutorials
cd trueblocks-tutorials
mkdir -p build && cd build
pwd                                             # should say ./trueblocks-tutorials/build
cmake ../src
make -j 4
ls -l ../bin                                    # should see an executable called chifra
```

### To test the build

Once TrueBlocks is built, run these commands to test your installation.

```bash
export PATH=../bin:$PATH
which chifra                    # it should say ../bin/chifra
chifra version                  # it should version 1.0.0 or later
chifra status                   # this may download a file from an IPFS gateway
```

### Running your own local node?

This lesson does not require you to run your own node, but we will be. Some info about that is in the recording.

----
**`[Show Reth and Docker]`**

[Reth Grafana Dashboard](http://localhost:3000)

----

### Lesson

In this lesson, we've built TrueBlocks, explored `chifra status` and shown that running a local Reth node through docker is simple.

### Next

Now, let's get some data.

- [Step 1 - Getting Started](step1)
- [Step 2 - Getting Data?](step2)
- [Step 3 - Getting Details](step3)
- [Step 4 - Getting Serious](step4)

### Save your PATH

It makes sense to save the change to your PATH so each time you log in to a shell, you don't have to repeat the above `export`. We leave that as an exercise for you.
