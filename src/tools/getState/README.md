## chifra state

Use this tool to retrieve the balance of an address (or list of addresses) at the given block (or blocks). Specify multiple addresses and/or multiple blocks if you wish, but you must specify at least one address. If no block is specified, the latest block is reported.

You may also query to see if an address is a smart contract as well as retrieve a contract's byte code.

### Usage

`Usage:`    chifra state [-p|-c|-n|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Retrieve account balance(s) for one or more addresses at given block(s).

`Where:`  

|     | Option              | Description                                                                                                                              |
| --- | ------------------- | ---------------------------------------------------------------------------------------------------------------------------------------- |
|     | addrs               | one or more addresses (0x...) from which to retrieve balances (required)                                                                 |
|     | blocks              | an optional list of one or more blocks at which to report balances, defaults to 'latest'                                                 |
| -p  | --parts &lt;val&gt; | control which state to export, one or more of *[ none \| some\* \| all \| balance \| nonce \| code \| storage \| deployed \| accttype ]* |
| -c  | --changes           | only report a balance when it changes from one block to the next                                                                         |
| -n  | --no_zero           | suppress the display of zero balance accounts                                                                                            |
| -x  | --fmt &lt;val&gt;   | export format, one of *[ none \| json\* \| txt \| csv \| api ]*                                                                          |
| -v  | --verbose           | set verbose level (optional level defaults to 1)                                                                                         |
| -h  | --help              | display this help screen                                                                                                                 |

`Notes:`

- An `address` must start with '0x' and be forty-two characters long.
- `blocks` may be a space-separated list of values, a start-end range, a `special`, or any combination.
- If the queried node does not store historical state, the results are undefined.
- `special` blocks are detailed under `chifra when --list`.
- `balance` is the default mode. To select a single mode use `none` first, followed by that mode.
- You may specify multiple `modes` on a single line.

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

| Command     | Description                                                                                     |
| ----------- | ----------------------------------------------------------------------------------------------- |
| --version   | display the current version of the tool                                                         |
| --nocolor   | turn off colored display                                                                        |
| --wei       | specify value in wei (the default)                                                              |
| --ether     | specify value in ether                                                                          |
| --dollars   | specify value in US dollars                                                                     |
| --raw       | report JSON data from the node with minimal processing                                          |
| --very_raw  | report JSON data from node with zero processing                                                 |
| --fmt       | export format (where appropriate). One of [ none &#124; txt &#124; csv &#124; json &#124; api ] |
| --to_file   | write the results to a temporary file and return the filename                                   |
| --output:fn | write the results to file 'fn' and return the filename                                          |
| --file:fn   | specify multiple sets of command line options in a file.                                        |

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source code**: [`tools/getState`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getState)

