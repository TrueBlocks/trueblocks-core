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
  -l, --call string        call a smart contract with one or more solidity calls, four-byte plus parameters, or encoded call data strings
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
  - You may specify multiple parts on a single line.
  - In the --call string, you may separate multiple calls with a colon.
```

Data models produced by this tool:

- [function](/data-model/other/#function)
- [message](/data-model/other/#message)
- [parameter](/data-model/other/#parameter)
- [result](/data-model/chainstate/#result)
- [state](/data-model/chainstate/#state)

### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --output string   write the results to file 'fn' and return the filename
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
```

**Note:** For the `--file string` option, you may place a series of valid command lines in a file using any
valid flags. In some cases, this may significantly improve performance. A semi-colon at the start
of any line makes it a comment.

**Note:** If you use `--output --append` option and at the same time the `--file` option, you may not switch
export formats in the command file. For example, a command file with two different commands, one with `--fmt csv`
and the other with `--fmt json` will produce both invalid CSV and invalid JSON.

*Copyright (c) 2024, TrueBlocks, LLC. All rights reserved. Generated with goMaker.*
