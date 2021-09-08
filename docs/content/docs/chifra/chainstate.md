---
title: "Chain State"
description: ""
lead: ""
date: 2021-05-08T01:35:20
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu:
  docs:
    parent: "chifra"
weight: 1500
toc: true
---
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
## chifra state

Use this tool to retrieve the balance of an address (or list of addresses) at the given block (or blocks). Specify multiple addresses and/or multiple blocks if you wish, but you must specify at least one address. If no block is specified, the latest block is reported.

You may also query to see if an address is a smart contract as well as retrieve a contract's byte code.

### usage

`Usage:`    chifra state [-p|-c|-n|-v|-h] &lt;address&gt; [address...] [block...]
`Purpose:`  Retrieve account balance(s) for one or more addresses at given block(s).

`Where:`

| | Option | Description |
| :----- | :----- | :---------- |
|  | addrs | one or more addresses (0x...) from which to retrieve balances (required) |
|  | blocks | an optional list of one or more blocks at which to report balances, defaults to 'latest' |
| -p | --parts &lt;val&gt; | control which state to export, one or more of *[ none \| some\* \| all \| balance \| nonce \| code \| storage \| deployed \| accttype ]* |
| -c | --changes | only report a balance when it changes from one block to the next |
| -n | --no_zero | suppress the display of zero balance accounts |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- An `address` must start with '0x' and be forty-two characters long.
- `blocks` may be a space-separated list of values, a start-end range, a `special`, or any combination.
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `chifra when --list`.
- `balance` is the default mode. To select a single mode use `none` first, followed by that mode.
- You may specify multiple `modes` on a single line.

**Source code**: [`tools/getState`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getState)

## chifra tokens

Given the address of an ERC20 token contract, this tool reports token balances for one or more additional addresses. Alternatively, the tool can report the token balances for multiple ERC20 tokens for a single addresses.

In normal operation the **first item** in the `address_list` is assumed to be an ERC20 token contract whose balances are being queried, whereas the remainder of the list is assumed to be addresses on which to report.

In `--byAcct` mode, **all addresses** in the `address_list` are assumed to be ERC20 token contracts, except the final one which is the account whose token balances are reported.

You may optionally specify one or more blocks at which to report. If no block is specified, the latest block is assumed. You may also optionally specify which parts of the token data to extract.

### usage

`Usage:`    chifra tokens [-p|-b|-n|-v|-h] &lt;address&gt; &lt;address&gt; [address...] [block...]
`Purpose:`  Retrieve token balance(s) for one or more addresses at given block(s).

`Where:`

| | Option | Description |
| :----- | :----- | :---------- |
|  | addrs | two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported (required) |
|  | blocks | an optional list of one or more blocks at which to report balances, defaults to 'latest' |
| -p | --parts &lt;val&gt; | one or more parts of the token information to retreive, one or more of *[ name \| symbol \| decimals \| totalSupply \| version \| none \| all\* ]* |
| -b | --by_acct | consider each address an ERC20 token except the last, whose balance is reported for each token |
| -n | --no_zero | suppress the display of zero balance accounts |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- An `address` must start with '0x' and be forty-two characters long.
- `blocks` may be a space-separated list of values, a start-end range, a `special`, or any combination.
- If the token contract(s) from which you request balances are not ERC20 compliant, the results are undefined.
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `chifra when --list`.

**Source code**: [`tools/getTokens`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getTokens)

