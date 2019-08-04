## ethslurp

`ethslurp` is one of our older command line tools. It even has its [own website](http://ethslurp.com) and [video tutorial] (https://www.youtube.com/watch?v=ZZDV1yAgces). While this tool is very useful, it has two significant flaws. First, it is not decentralized--in fact it is fully centralized, pulling its data from [http://etherscan.io](http://etherscan.io) as it does. The tool's second major flaw is that it does not pull every transaction from the chain for a given account as our [account monitors](../../monitors/README.md) do. This is because of `internal transactions` which, believe us, is too complicated to be worth taking the time to explain.

While `ethslurp` does have its shortcomings, it also provides some nice features. You can use it to pull all externally generated transactions against an account. You may also use the tool's fully customizable formatting options to export data into any format you wish. Visit the above referenced website for more information.

#### Usage

`Usage:`    ethslurp [-t|-b|-s|-x|-v|-h] &lt;address&gt; [address...] [block...]  
`Purpose:`  Fetches data from EtherScan for an arbitrary address. Formats the output to your specification.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addr_list | one or more addresses to slurp (required) |
|  | block_list | an optional range of blocks to slurp |
| -t | --type enum[ext|int|token|miner|all] | extract either [ ext &#124; int &#124; token &#124; miner &#124; all ] type of transactions |
| -b | --blocks <range> | export records in block range (:0[:max]) |
| -s | --silent | Run silently (only freshen the data, do not display it) |
| -x | --fmt <fmt> | pretty print, optionally add ':txt,' ':csv,' or ':html' |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Portions of this software are Powered by Etherscan.io APIs.

#### Other Options

All **QBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --raw       |   report JSON data from the node with minimal processing
    --veryRaw   |   report JSON data from node with zero processing
    --fmt       |   export format (where appropriate). One of [none|txt|csv|json|api]
    --api_mode  |   simulate api_mode for testing
    --to_file   |   write the results to a temporary file and return the filename
    --output:fn |   write the results to file 'fn' and return the filename
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**


