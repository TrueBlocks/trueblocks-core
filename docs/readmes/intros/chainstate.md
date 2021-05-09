## intro

The two tools in this group deal with the Ethereum Chain State which may be distinquished from Ethereum Chain Data such as blocks, transactions, or traces.

There are two tools, `chifra state` and `chifra tokens`. The first allows you to query account balances, the byte code of a smart contract (if available), the nonce and other information about any address. The second tool, `chifra tokens`, deals with token balances and ERC20 tokens information (and soon ERC721 tokens).

The amount of information you can retrieve from the node depends on what type of node you're running (`--tracing`, `archive`, `--tracing archive` or `full node`). Archive nodes and tracing allow you to query historical data and balances. Non-archive nodes work, but are much less informative.
