ABI files are derived from the Solidity source code of a smart contract by extracting the canonical function and event signatures in a JSON structure. The function signatures are hashed (using keccak) into four-byte encodings for functions and 32-byte encodings for events. Because the blockchain only deals with byte data, TrueBlocks needs a way to decode the bytes back into the human-readable function and event signatures. We call this process `--articulate`. Most TrueBlocks commands provide an `--articulate` option. See the commands themselves for more information.

The following commands produce and manage [{PLURAL}]:

| Tools                                                 |                                                              |
| ----------------------------------------------------- | ------------------------------------------------------------ |
| [chifra export](/docs/chifra/accounts/#chifra-export) | export full detail of transactions for one or more addresses |
| [chifra abis](/docs/chifra/accounts/#chifra-abis)     | fetches the ABI for a smart contract                         |

[{PROPER}] data is made of the following data fields:

[{FIELDS}]
