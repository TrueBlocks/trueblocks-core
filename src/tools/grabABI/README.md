## chifra abis

`chifra abis` retrieves a smart contract's ABI file either from the current local folder, from the TrueBlocks cache, from [Etherscan](http://etherscan.io), or (in the future) from ENS and Sourcify.

While this tool may be used from the command line or the API, its primary use is in support of the `--articulate` option of other tools such as `chifra export`.

The `--known` option prints a list of standard and semi-standard function signatures such as the ERC20 standard, various OpenZeppelin functions, the base and pair Uniswap functions, etc.

The `--find` option is experimental. It scans a cross of approx. 100,000 function names with approx. 700 function signatures, generating at each cross a four byte signature and looking for a match to the provided fourbyte. Very infrequently, this tool will find a match for an otherwise unknown fourbyte string.

The `--sol` option will convert a single Solidity file found in the current folder into an ABI.

### Usage

`Usage:`    chifra abis [-k|-s|-f|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Fetches the ABI for a smart contract.

`Where:`  

|     | Option             | Description                                                                     |
| --- | ------------------ | ------------------------------------------------------------------------------- |
|     | addrs              | list of one or more smart contracts whose ABI to grab from EtherScan (required) |
| -k  | --known            | load common 'known' ABIs from cache                                             |
| -s  | --sol &lt;str&gt;  | file name of .sol file from which to create a new known abi (without .sol)      |
| -f  | --find &lt;str&gt; | try to search for a function declaration given a four byte code                 |
| -x  | --fmt &lt;val&gt;  | export format, one of *[ none \| json\* \| txt \| csv \| api ]*                 |
| -v  | --verbose          | set verbose level (optional level defaults to 1)                                |
| -h  | --help             | display this help screen                                                        |

`Notes:`

- Solidity files found in the local folder with the name '<address>.sol' are converted to an ABI prior to processing (and then removed).

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

**Source code**: [`tools/grabABI`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/grabABI)

