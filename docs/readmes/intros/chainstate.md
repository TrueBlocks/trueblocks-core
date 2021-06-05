The two tools in this group deal with the _Chain State_.
As chain state data concerns balances and byte code.
it is distinct from Chain Data, which concerns things like blocks, transactions,
or traces.

The two tools are `chifra state` and `chifra tokens`.
The first allows you to query account balances, the byte code of a smart contract (if available), the nonce, and other information about an address.
The second tool, `chifra tokens`, deals with ERC20 and ERC721 token balances and other data.

{{< alert icon="👉" title="info depends on node"
text="Note: The amount of information you can retrieve depends on the type of node you run. Archive nodes and tracing allow you to query historical state (that is, all the way back to the genesis block). TrueBlocks works with non-archive nodes, but they are much less informative."
>}}
