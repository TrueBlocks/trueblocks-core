## chifra state

Use this tool to retrieve the balance of an address (or list of addresses) at the given block (or blocks). Specify multiple addresses and/or multiple blocks if you wish, but you must specify at least one address. If no block is specified, the latest block is reported.

You may also query to see if an address is a smart contract as well as retrieve a contract's byte code.

```[plaintext]
Purpose:
  Retrieve account balance(s) for one or more addresses at given block(s).

Usage:
  chifra state [flags] <address> [address...] [block...]

Arguments:
  addrs - one or more addresses (0x...) from which to retrieve balances (required)
  blocks - an optional list of one or more blocks at which to report balances, defaults to 'latest'

Flags:
  -p, --parts strings   control which state to export
                        One or more of [ none | some | all | balance | nonce | code | storage | deployed | accttype ]
  -c, --changes         only report a balance when it changes from one block to the next
  -n, --no_zero         suppress the display of zero balance accounts

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - An address must start with '0x' and be forty-two characters long.
  - blocks may be a space-separated list of values, a start-end range, a special, or any combination.
  - If the queried node does not store historical state, the results are undefined.
  - special blocks are detailed under chifra when --list.
  - balance is the default mode. To select a single mode use none first, followed by that mode.
  - You may specify multiple modes on a single line.
```

**Source code**: [`tools/getState`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getState)

