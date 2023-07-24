## chifra state

<!-- markdownlint-disable MD041 -->
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
                           One or more of [ none | some | all | balance | nonce | code | proxy | deployed | accttype ]
  -c, --changes            only report a balance when it changes from one block to the next
  -z, --no_zero            suppress the display of zero balance accounts
  -a, --call string        call a smart contract with a solidity syntax, a four-byte and parameters, or encoded call data
  -r, --proxy_for string   for the --call option only, redirects calls to this implementation
  -H, --ether              specify value in ether
  -o, --cache              force the results of the query into the cache
  -x, --fmt string         export format, one of [none|json*|txt|csv]
  -v, --verbose            enable verbose (increase detail with --log_level)
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

- [callresult](/data-model/chainstate/#callresult)
- [ethcall](/data-model/chainstate/#ethcall)

Links:

- [api docs](/api/#operation/chainstate-state)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/state)

