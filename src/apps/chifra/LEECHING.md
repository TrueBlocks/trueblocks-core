## Leeching the TrueBlocks Index

**TrueBlocks** indexes the Ethereum blockchain. Given this index, you may extact a full and detailed transaction history of your accounts and smart contracts. **TrueBlocks** is *not* a replacement for Web3.js (yet!). These instruction will help you use the index.

### Downloading and installing TrueBlocks

Using the index requires you to make the TrueBlocks tools available on your comptuer. We assume you are familiar with your computer's command line. In your development folder, do:

```
git clone -b develop https://github.com/Great-Hill-Corporation/trueblocks-core.git
cd trueblocks-core
export PATH=$PATH:`pwd`/bin
mkdir build && cd build
cmake ../src
make
```
If you have trouble with any part of the build, please see the [full installation instructions](x).

**Aside:** IF you export the path `./trueblocks-core/bin` during your shell's initialization, the command-line tools will be available everywhere. The following instructions assume you've done this.

### Testing the installation

If you're running a local Ethereum node and you've added `../trueblocks-core/bin` to your `$PATH`, you should be able to execute:

```
./getBlock latest
```

and receive JSON data from the lastest Ethereum block.
