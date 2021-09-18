Parameters are a constituant part of a [Function or Event](/data-model/accounts/#function). The parameters of a function are each individual value passed into the function. Along with the function's name, the parameters types (once canonicalized) are used to create a function's four byte signature (or an event's 32-byte signature). Parameters are important to TrueBlocks because we use them as part of the ABI decoding and the `--articulate` process to conver the blockchain's bytes into human-readable text.

[{FIELDS}]

| Tools                                             |                                      |
| ------------------------------------------------- | ------------------------------------ |
| [chifra abis](/docs/chifra/accounts/#chifra-abis) | fetches the ABI for a smart contract |
