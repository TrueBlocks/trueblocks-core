Receipts record the amount of gas used for a transaction among other things. If the transaction succeeded, a receipt might also have logs.

If the `to` address of a transaction is `0x0`, the `input` data is considered to be the source code (byte code) of a smart contract. In this case, if the creation of the contract succeeds, the `contractAddress` field of the receipt carries the address of the newly created contract.

The following commands produce and manage [{PLURAL}]:

| Tools                                                      |                                                |
| ---------------------------------------------------------- | ---------------------------------------------- |
| [chifra receipts](/docs/chifra/chaindata/#chifra-receipts) | retrieve receipts for the given transaction(s) |

[{PROPER}] data is made of the following data fields:

[{FIELDS}]
