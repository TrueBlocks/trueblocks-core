ABI files are derived from the Solidity source code of a smart contract by extracting the canonical function and event signatures in a JSON structure. The function signatures are hashed (using keccak) into four-byte encodings for functions and 32-byte encodings for events. Because the blockchain only deals with byte data, TrueBlocks needs a way to decode the bytes back into the human-readable function and event signatures. We call this process `--articulate`. Most TrueBlocks commands provide an `--articulate` option. See the commands themselves for more information.

[{FIELDS}]

| **Tools**                                             |                                                              |
| ----------------------------------------------------- | ------------------------------------------------------------ |
| [chaindata commands](/docs/chifra/chaindata/)         | various commands dealing with blockchain data                |
| [chifra export](/docs/chifra/accounts/#chifra-export) | export full detail of transactions for one or more addresses |
