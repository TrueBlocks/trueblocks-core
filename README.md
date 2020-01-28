### Important Notes:

1. TrueBlocks is alpha code. This means that we will be making breaking changes to the code and data as we move forward. Be forewarned.
2. We have changed the name of our project to TrueBlocks. You may see reference to QuickBlocks here and there as we clean up the repo.

-----------------

### Contact Info

Website: [http://trueblocks.io](http://trueblocks.io)  

#### Installing QBlocks

First things first. We start with the [installation instructions](src/other/install/INSTALL.md).

#### What is QBlocks?

QBlocks is a collection of software [libraries](src/libs), [applications](src/apps), [tools](src/tools), and [examples](src/examples) that allow you to retrieve
Ethereum blockchain data (a) more quickly, (b) with higher information content, (c) in an fully decentralized way, (d) in a fully automated way, and (e) in a highly 
maintenance free way. The many potential use cases for QBlocks are described
[here](https://quickblocks.io/docs/usecases.html).

#### More Quickly

QBlocks interacts with the Ethereum blockchain via the RPC interface to accumulate `blocks`, `transactions`, `receipts`, and event `logs`. Prior to storing the
data, we optimize it for later retrieval. The primary goal of QBlocks is to return the data as quickly as possible while maintaining a fully decentralized
stance. These optimizations include:

1. attaching `receipts` and `logs` directly to `transactions`,
2. identifying in-error transactions using the node's trace functionality,
3. pre-processing incoming internal transactions,
4. storing the data optimized for quick, later retrieval.

Depending on the use case, QBlocks is able to achieve more than 150 times speedup relative to the node's RPC interface.

#### Higher Information Content

An Ethereum node knows nothing about Solidity source code. It only deals with byte code and therefore can only return hexadecimal bytes. In order to make the data useful, each user must translate the data back into the language of the smart contract. For example, this data:

    {
      "hash": "0xb9f174f61ef78661df519dba92f7a724593d1376e18edc8f2cc477684f089d13",
      "blockNumber": 2243947,
      "transactionIndex": 9,
      "timestamp": 1473657968,
      "from": "0x80f10e72de8a35d57efe477c1d1bd147be50307d",
      "to": "0xbb9bc244d798123fde783fcc1c72d3bb8c189413",
      "value": 0,
      "input": "0xc9d27afe00000000000000000000000000000000000000000000000000000000000001290000000000000000000000000000000000000000000000000000000000000001",
    }

is simply vote on a DAO contract:

    {
      "from": "0x80f10e72de8a35d57efe477c1d1bd147be50307d",
      "vote": {
        "proposal": 297,
        "supports": 1
      }
    }

By default, QBlocks returns the raw hexadecimal data identical to the RPC, but with a simple switch, it can return what we call `articulated data` or data in the language of your smart contract.

QBlocks is able to articulate the data by using the smart contract's ABI file. If available, an ABI file contains all the information necessary to translate the
RPC data back into the language of the smart contract. This functionality is implemented in the open source [grabABI](src/apps/grabABI)
application. Instead of dealing with **hashes** and **merkel roots** and **sha3**, you deal with **votes** and **transfers** and **withdrawals**.

#### Fully Decentralized

Unlike other block chain scrapers such as http://etherscan.io, QBlocks is, by default, fully decentralized. QBlocks interacts with only your local Ethereum
node. (Although, it can, if you wish, interact with any node such as Infura). This aspect of our system distinguishes QBlocks from other options. If you
understand and believe in the benefits of decentralization, you will understand why this is important. If you don't, you'll undervalue this aspect of our work.

#### Fully Automatic (Yet Customizable)

The source code for ***all of the functionality*** mentioned above is generated automatically (i.e. programmatically) using the [grabABI](src/apps/grabABI) and
[makeClass](src/apps/makeClass) apps. This makes standing up a [monitor](src/monitors/README.md) for any smart contract (or system of smart contracts) trivial. With
a one line command to [chifra](src/apps/chifra/README.md), a fully functional C++ static library is created. At the same time, because the generated code is fairly
simple C++ code, it is fully customizable.

#### Low Maintenance

Every smart contract, once deployed, is immutable. That means that the automatically generated C++ code is also immutable (if you want it to). You could literally
automatically generate a data delivery layer for your smart contract that requires no further interaction. It can simply run forever, standing off-chain, but beside
your smart contract, and deliver all your contact's data at high speed.

#### Structure of the Project

1. [src](src) - source code for the QBlocks libraries, core applications, tools, and sample monitors.
2. bin - location of primary executables generated by QBlocks
3. docs - documentation
