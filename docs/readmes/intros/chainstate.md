## intro

The two tools in this group deal with the Chain State such as balances and byte code which may be distinquished from Chain Data such as blocks, transactions, or traces.

The two tools are `chifra state` and `chifra tokens`. The first allows you to query account balances, the byte code of a smart contract (if available), the nonce and other information about an address. The second tool, `chifra tokens`, deals with ERC20 and ERC721 token balances and other data.

**Note:** The amount of information available for retrieval depends on the type of node you run (`--tracing`, `archive`, `--tracing archive` or `full node`). Archive nodes and tracing allow you to query historical state (that is, state all the way back to the genesis block). TrueBlocks works with non-archive nodes, but they are much less informative.
