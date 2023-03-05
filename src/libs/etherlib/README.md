<!-- markdownlint-disable MD041 -->
## etherlib

etherlib is at the heart of the entire system and consists of four  primary functionalities: (1) communication with the blockchain node, (2) mirroring and caching the received blockchain data, (3) parsing of both transactional input data and receipt logs addresses and topics (it is this function that underpins the tokenlib and per-smart-contract libraries), (4) a series of convenience functions for traversing all or part of the blocks and the transactions and accounts encountered while traversing the blocks. A final functionality is the acquisition and application of the currency’s price data.

### Communication with the node

Using either the RPC via the curl libraries or inter-process communication, the etherlib communicates with the node using functions similarly named to those found in the RPC. For example, functions getBlocks, getTrans, getReceipts, traceTransaction, latestBlock, and getState mirror names from the node’s RPC.

Many of the interfaces provided by the RPC for retrieving data are mimicked by etherlib. We do not provide interfaces for communicating back to the node to do things such as sign or initiate transactions. We feel that this type of interaction is best handled by secured wallet software or some other means where there has been a much heavier focus on security. While having done everything we can to make our code secure, our system is dealing only with immutable data extraction from the blockchain.

### Mirroring of blockchain data

The mirroring (or caching) of the blockchain data is provided by six classes: CBlock, CTransaction, CReceipt, CLog, CTrace, and CStructLog. These classes are documented in our [white papers](../../other/papers/README.md).

As has been discussed earlier, the various fields in the CBlock class include a list of transactions. The CTransaction class, the most used class in the system, contains the from, to, value, and timestamp values expected of any transaction in addition to a transaction receipt. The CReceipt class contains, importantly, gasUsed and the logs and traces necessary to determine the outcome of a transaction.

### Parsing of transaction input and event topics

Given an ABI for a smart contract, we first ensure that the function and event signatures are canonical. Given the canonical signatures we then proceed to encode these signatures so they may be used later to decode the input data fields of each transaction and the indexed topics and data of each receipt log.

This involves a two-step process of first converting the Ascii representation of the canonical signature to hexadecimal and then using RPC’s sha3 function to encrypt the signature. For smart contract functions found in the ABI, only the first four bytes (eight hexadecimal characters of the resulting sha3) are used as the encoding. For event in the ABI, the entire 32 bytes (64 hexadecimal characters) are used.

Armed with the encoded input and event signatures, the system uses the ABI to decode the input data field. It does this by reading the type of each parameter to a function or event and extracting that data type from the input field (in the case of a transaction) or in the indexed topics (in the case of the receipt logs).

Armed with this extracted data, the function parser returns a delimited string of the function or event name plus the various parameters.

The tokenlib, walletlib, and customized per-smart-contract libraries further use this parsed function and event data to promote particular transactions or receipt logs to a customized, recognizable, C++ class fully exposing the function or event’s meaning.

### Convenience functions for traversing blocks, transactions, and accounts.

There are numerous functions in the etherlib that begin with forEvery…, such as forEveryBlockFromClient, or forEveryMini-Block. There are too many to enumerate here. Please see our [white papers](../../other/papers/README.md) for a complete list.

These functions expect to have been previously instructed how the end user wishes to access the requested items by use of an initialization function called etherlib_init().

The initialization function may be called with values such as “infura,” “parity,” “geth,” “binary,” or “fastest.” The system determines how best to deliver the data requested in the most efficient way.

For example, because TrueBlocks™ does not store input data in the fixed-width miniTransaction arrays (because it is of variable length), we fall back to the binary representation if that data is requested.

Furthermore, if the system is directed to deliver blocks containing no transactions, it reverts to requesting that data from the RPC. In this way, the etherlib is able to always deliver the requested data as quickly as possible. Thereby comes our system’s name— TrueBlocks™.

## abilib

The abilib library provides capabilities for reading, writing, and manipulating ABI (application binary interface) files. These files are generated by the Solidity compilers and aid in the deployment of Ethereum smart contracts. An ABI file lists each smart contract interface function and all events that may be emitted by the smart contract. Both public and internal, constant and non-constant function calls are included in an ABI, however because only non-constant, public functions lay transactions on the chain, the `abilib` only concerns itself with that subset of functions. All events found in the ABI are of interest.

## What this library does

`Abilib` is the primary component of two significant portions of our system: (1) the ABI data for a smart contract is used when parsing the input data field of a blockchain transaction, thereby associating the appropriate function call to the transaction awaiting further use. It does this by providing the parameter lists to the function as found in the ABI in an easy to use format. Additionally, event encoding and parameter data is parsed using the similar functionality. It is this ability to p arse a transaction’s input data and an events logs that allows us to promote generic transactions to per-smart-contract classes as mentioned above. This realizes the second major function of the abilib library.

Three classes comprise the `abilib` library:

## CAbi

The primary class in the library is CAbi which carries the list of functions and events found in the smart contract’s interface. We choose to record this list of functions and events twice, once sorted by name, and a second list sorted by the item’s encoding. In this way, we allow ourselves much faster access to the data than would be possible otherwise. In some parts of the processing, we need to find an encoding given a name, in others we need to find a name given an encoding; therefore, we store the list twice, sorted by either name or encoding.

## CFunction

A second class, stored as the above mentioned two lists, is the CFunction class. CFunction stores both the functions and the events of a smart contract.

## CParameter

CFunctions store lists of CParameters which are tuples of field types, field names and other attributes. In this way, the CAbi class is able to represent a full description of the public facing interface of any smart contract. Each of these three classes in the abilib library were generated automatically from configuration files using the makeClass application as detailed in our [white papers](../../other/papers/README.md).

## Auto-generated Code

By nature of having been derived from the CBaseNode class, each of these classes implement the ability to serialize and deserialize themselves, display themselves using display strings, and import and export themselves to various file formats.

In our [white papers](../../other/papers/README.md), we’ve included the configuration files for the abilib classes, as well as the definition files for all of the automatically generated code in the etherlib and tokenlib libraries. Because most of the code is automatically generated in these libraries, we show only the configuration files in the appendix.
