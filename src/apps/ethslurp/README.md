## ethslurp

`ethslurp` is one of our older command line tools. It even has its [own website](http://ethslurp.com) and [video tutorial] (https://www.youtube.com/watch?v=ZZDV1yAgces). While this tool is very useful, it has two significant flaws. First, it is not decentralized--in fact it is fully centralized, pulling its data from [http://etherscan.io](http://etherscan.io) as it does. The tool's second major flaw is that it does not pull every transaction from the chain for a given account as our [account monitors](../../monitors/README.md) do. This is because of `internal transactions` which, believe us, is too complicated to be worth taking the time to explain.

While `ethslurp` does have its shortcomings, it also provides some nice features. You can use it to pull all externally generated transactions against an account. You may also use the tool's fully customizable formatting options to export data into any format you wish. Visit the above referenced website for more information.

#### Usage

`Usage:`    ethslurp [-a|-b|-d|-f|-i|-e|-v|-h] addr  
`Purpose:`  Fetches data off the Ethereum blockchain for an arbitrary account or smart contract. Optionally formats the output to your specification. Note: --income and --expense are mutually exclusive as are --blocks and --dates.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addr | the address of the account or contract to slurp |
| -a | --archive val | filename of output (stdout otherwise) |
| -b | --blocks range | export records in block range (:0[:max]) |
| -d | --dates val | export records in date range (:yyyymmdd[hhmmss][:yyyymmdd[hhmmss]]) |
| -f | --fmt val | pretty print, optionally add ':txt,' ':csv,' or ':html' |
| -i | --income | include income transactions only |
| -e | --expense | include expenditures only |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Portions of this software are Powered by Etherscan.io APIs.

#### Other Options

All **quickBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by QuickBlocks<sup>&reg;</sup>**


