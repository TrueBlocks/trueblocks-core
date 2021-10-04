An ABI describes an Application Binary Interface -- in other words, the [Function](/data-model/other/#function) and Event signatures for a given smart contract. Along with [Names](/data-model/accounts/#names) the use of ABIs goes a very long way towards making your Ethereum data much more understandable.

Similar to names of addresses, ABI files are not available on-chain which means they must be acquired somewhere. Unfortunately, the Ethereum community has not yet understand that EtherScan is not a good place to store this very important information. For this reason, TrueBlocks uses EtherScan to acquire ABI files and therefor one needs to get an EtherScan API key to use this function.

The following commands produce and manage [{PLURAL}]:

| Tools                                             |                                      |
| ------------------------------------------------- | ------------------------------------ |
| [chifra abis](/docs/chifra/accounts/#chifra-abis) | fetches the ABI for a smart contract |

[{PROPER}] data is made of the following data fields:

[{FIELDS}]

**Notes**

See the `chifra abis` command line for information about getting an EtherScan key.
