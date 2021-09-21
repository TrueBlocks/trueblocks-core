When exported with the `--accounting` option from `chifra export`, each transaction will have field called `statements`. Statements are an array for reconciliations. All such exported transactions will have at least one reconcilation (for ETH), however, many will have additional reconciliations for other assets (such as ERC20 and ERC721 tokens).

Because DeFi is essentially swaps and trades around ERC20s, and because and 'programmable money' allows for unlimited actions to happen under a single transaction,many times a transaction has four or five reconciliations.

Reconciliations are relative to an `accountedFor` address. For this reason, the same transaction will probably have different reconciliations depending on the `accountedFor` address. Consider a simple transfer of ETH from one address to another. Obviously, the sender's and the recipient's reconciliations will differ (in opposite proportion to each other). The `accountedFor` address is always present as the `assetAddress` in the first reconciliation of the statements array.

The following commands produce and manage [{PLURAL}]:

| Tools |     |
| ----- | --- |

[{PROPER}] data is made of the following data fields:

[{FIELDS}]

**Notes**

**Intra-block transactions**: In many cases two or more transactions requiring a reconciliation may occur in a single block. Becuase the Ethereum blockchain only provides balance queries at the end of blocks, it is not possible to query for the balance of an asset at the end of transactions for which there are other following transactions in the block nor for the beginning balance for which there are transactions prior to the given transaction in the same block. In these cases, TrueBlocks simulates the beginning and ending balance as needed and adds `partial` to the `reconciliationType`.

**Spot Price**: If the `spotPrice` is available from an on-chain source (such as UniSwap), then it represents the ETH/DAI value at the time of the transaction if the reconcilation is for ETH. For other assets, the `spotPrice` represents the asset's value relative to `ETH`, so to price a non-ETH asset in US dollars, one would need to convert first to `ETH` then to dollars. If a price is not available on-chain, the `spotPrice` will be zero and the caller is encouraged to get the price for the asset from other sources.
