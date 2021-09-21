Logs appear in a possibly empty array in the transaction's receipt. They are only created if the underlying transaction suceeded. In the case where the transaction failed, no logs will appear in the receipt. Logs are only ever generated during transactions whose `to` address is a smart contract.

The following commands produce and manage [{PLURAL}]:

| Tools                                              |                                            |
| -------------------------------------------------- | ------------------------------------------ |
| [chifra logs](/docs/chifra/chaindata/#chifra-logs) | retrieve logs for the given transaction(s) |

[{PROPER}] data is made of the following data fields:

[{FIELDS}]
