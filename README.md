# TrueBlocks Core

![GitHub repo size](https://img.shields.io/github/repo-size/scottydocs/README-template.md)
![GitHub contributors](https://img.shields.io/github/contributors/scottydocs/README-template.md)
![GitHub stars](https://img.shields.io/github/stars/Great-Hill-Corporation/trueblocks-core?style%3Dsocial)
![GitHub forks](https://img.shields.io/github/forks/Great-Hill-Corporation/trueblocks-core?style=social)
![Twitter Follow](https://img.shields.io/twitter/follow/trueblocks?style=social)

TrueBlocks allows you to build local-first, fully-decentralized applications using data directly from an Ethereum node. It does this through two mechanisms:

1. A lightning-fast index of every appearance of every addresses on the chain, and
2. A binary cache of only the data your application extracts. 

Local-first means your application is **private by default**, the client-side cache means your application is **fast**, and *extraction-on-demand* means your application will remain **minimal**.

[How does TrueBlocks Work?](docs/FAQ.md#how-it-works)

## Prerequisites

Before building TrueBlocks, complete these prerequisites:

#### On Linux:

```[shell]
sudo apt install build-essential git cmake python python-dev libcurl3-dev
```

#### On Mac:

```[shell]
brew install cmake
brew install git
```
## Building TrueBlocks

Currently, you must build TrueBlocks from source:

```[shell]
git clone git@github.com:TrueBlocks/trueblocks-core.git
cd trueblocks-core
mkdir build && cd build
cmake ../src
make
```

This will create a series of executables in the `./bin` folder at the top of the repo. The following instructions assume you've added this folder to your `$PATH`.

[Does TrueBlocks Work on Windows?](docs/FAQ.md-windows)

## Using TrueBlocks

After building, test your configuration with this command:

```[shell]
chifra --version
```

To get a complete list of available commands, do this:

```[shell]
chifra --help --verbose
```

[Where to Go from Here](docs/FAQ.md#next-steps)

## Contributing to TrueBlocks

1. Fork this repository.
2. Create a branch: `git checkout -b <branch_name>`.
3. Make changes and commit them: `git commit -m '<commit_message>'`
4. Push to the original branch: `git push origin TrueBlocks/trueblocks-core`
5. Create the pull request.

## Contributors

Thanks to the following people who have contributed to this project:

* [@tjayrush](https://github.com/tjayrush) 📖
* [@wildmolasses](https://github.com/wildmolasses) 📖

## Contact

If you have specific requests, contact us here <info@quickblocks.io>.

## License

This project licensed under the [Apache License Version 2.0](LICENSE.md).
