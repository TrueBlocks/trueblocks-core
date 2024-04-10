When exported with the `--accounting` option from `chifra export`, each transaction will have field
called `statements`. Statements are an array for reconciliations. All such exported transactions
will have at least one reconciliation (for ETH), however, many will have additional reconciliations
for other assets (such as ERC20 and ERC721 tokens).

Because DeFi is essentially swaps and trades around ERC20s, and because and 'programmable money'
allows for unlimited actions to happen under a single transaction, many times a transaction has
four or five reconciliations.

Reconciliations are relative to an `accountedFor` address. For this reason, the same transaction
will probably have different reconciliations depending on the `accountedFor` address. Consider a
simple transfer of ETH from one address to another. Obviously, the sender's and the recipient's
reconciliations will differ (in opposite proportion to each other). The `accountedFor` address
is always present as the `assetAddress` in the first reconciliation of the statements array.
