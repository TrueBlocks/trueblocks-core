---
title: "Chain state"
description: "Access to account and token state"
lead: ""
lastmod:
  - :git
  - lastmod
  - publishDate
draft: false
aliases:
 - "/docs/chifra/chainstate"
menu:
  chifra:
    parent: commands
weight: 31000
toc: true
---

The two tools in this group deal with the Chain State. As chain state data concerns balances and
byte code. it is distinct from Chain Data, which concerns things like blocks, transactions, or
traces.

`chifra state` allows you to query the ETH account balance for an address, the byte code of a
smart contract (if available), the nonce, and other information about an address. The second tool,
`chifra tokens`, deals with ERC20 and ERC721 token balances and related data.

To the right is a list of commands in this group. Click on a command to see its full documentation.

Note: The amount of information you can retrieve depends on the type of node you run. Archive nodes
and tracing allow you to query historical state (that is, all the way back to the genesis block).
TrueBlocks works with non-archive nodes, but they are much less informative.

## chifra state

The `chifra state` tool retrieves the balance of an address (or list of addresses) at the given block
(or blocks). Specify multiple addresses and/or multiple blocks if you wish, but you must specify
at least one address. If no block is specified, the latest block is reported.

You may also query to see if an address is a smart contract as well as retrieve a contract's
byte code.

```[plaintext]
Purpose:
  Retrieve account balance(s) for one or more addresses at given block(s).

Usage:
  chifra state [flags] <address> [address...] [block...]

Arguments:
  addrs - one or more addresses (0x...) from which to retrieve balances (required)
  blocks - an optional list of one or more blocks at which to report balances, defaults to 'latest'

Flags:
  -p, --parts strings      control which state to export
                           One or more of [ balance | nonce | code | proxy | deployed | accttype | some | all ]
  -c, --changes            only report a balance when it changes from one block to the next
  -z, --no_zero            suppress the display of zero balance accounts
  -l, --call string        call a smart contract with a solidity syntax, a four-byte and parameters, or encoded call data
  -a, --articulate         for the --call option only, articulate the retrieved data if ABIs can be found
  -r, --proxy_for string   for the --call option only, redirects calls to this implementation
  -H, --ether              specify value in ether
  -o, --cache              force the results of the query into the cache
  -D, --decache            removes related items from the cache
  -x, --fmt string         export format, one of [none|json*|txt|csv]
  -v, --verbose            enable verbose output
  -h, --help               display this help screen

Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - Blocks is a space-separated list of values, a start-end range, a special, or any combination.
  - If the queried node does not store historical state, the results are undefined.
  - Special blocks are detailed under chifra when --list.
  - Balance is the default mode. To select a single mode use none first, followed by that mode.
  - Valid parameters for --call include Solidity-like syntax: balanceOf(0x316b...183d), a four-byte followed by parameters: 0x70a08231(0x316b...183d), or encoded input data.
  - You may specify multiple modes on a single line.
```

Data models produced by this tool:

- [result](/data-model/chainstate/#result)
- [state](/data-model/chainstate/#state)

Links:

- [api docs](/api/#operation/chainstate-state)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/state)

## chifra tokens

Given the address of an ERC20 token contract, the `chifra tokens` tool reports token balances for one or
more additional addresses. Alternatively, the tool can report the token balances for multiple ERC20
tokens for a single addresses.

In normal operation the **first item** in the `address_list` is assumed to be an ERC20 token
contract whose balances are being queried, whereas the remainder of the list is assumed to be
addresses on which to report.

In `--byAcct` mode, **all addresses** in the `address_list` are assumed to be ERC20 token contracts,
except the final one which is the account whose token balances are reported.

You may optionally specify one or more blocks at which to report. If no block is specified, the
latest block is assumed. You may also optionally specify which parts of the token data to extract.

```[plaintext]
Purpose:
  Retrieve token balance(s) for one or more addresses at given block(s).

Usage:
  chifra tokens [flags] <address> <address> [address...] [block...]

Arguments:
  addrs - two or more addresses (0x...), the first is an ERC20 token, balances for the rest are reported (required)
  blocks - an optional list of one or more blocks at which to report balances, defaults to 'latest'

Flags:
  -p, --parts strings   which parts of the token information to retrieve
                        One or more of [ name | symbol | decimals | totalSupply | version | some | all ]
  -b, --by_acct         consider each address an ERC20 token except the last, whose balance is reported for each token
  -c, --changes         only report a balance when it changes from one block to the next
  -z, --no_zero         suppress the display of zero balance accounts
  -o, --cache           force the results of the query into the cache
  -D, --decache         removes related items from the cache
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose output
  -h, --help            display this help screen

Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - Blocks is a space-separated list of values, a start-end range, a special, or any combination.
  - If the token contract(s) from which you request balances are not ERC20 compliant, the results are undefined.
  - If the queried node does not store historical state, the results are undefined.
  - Special blocks are detailed under chifra when --list.
  - If the --parts option is not empty, all addresses are considered tokens and each token's attributes are presented.
```

Data models produced by this tool:

- [token](/data-model/chainstate/#token)

Links:

- [api docs](/api/#operation/chainstate-tokens)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/tokens)

*Copyright (c) 2024, TrueBlocks, LLC. All rights reserved. Generated with goMaker.*
