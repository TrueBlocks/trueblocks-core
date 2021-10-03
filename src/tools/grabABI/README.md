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

|          | Option                             | Description                                                                    |
| -------- | ---------------------------------- | ------------------------------------------------------------------------------ |
|          | addrs                              | a list of one or more smart contracts whose ABIs to<br/>display (required)     |
| &#8208;k | &#8208;&#8208;known                | load common 'known' ABIs from cache                                            |
| &#8208;s | &#8208;&#8208;sol string           | file name of .sol file from which to create a new known<br/>abi (without .sol) |
| &#8208;f | &#8208;&#8208;find list&lt;str&gt; | try to search for a function declaration given a four<br/>byte code            |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;      | export format, one of [none, json, txt, csv, api]                              |
| &#8208;v | &#8208;&#8208;verbose              | set verbose level (optional level defaults to 1)                               |
| &#8208;h | &#8208;&#8208;help                 | display this help screen                                                       |

`Notes:`

- Solidity files found in the local folder with the name '<address>.sol' are converted to an ABI prior to processing (and then removed).

#### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

| Command     | Description                                                   |
| ----------- | ------------------------------------------------------------- |
| --version   | display the current version of the tool                       |
| --wei       | export values in wei (the default)                            |
| --ether     | export values in ether                                        |
| --dollars   | export values in US dollars                                   |
| --raw       | pass raw RPC data directly from the node with no processing   |
| --to_file   | write the results to a temporary file and return the filename |
| --output fn | write the results to file 'fn' and return the filename        |
| --file fn   | specify multiple sets of command line options in a file       |

<small>*For the `--file fn` option, you may place a series of valid command lines in a file using any of the above flags. In some cases, this may significantly improve performance. A semi-colon at the start of any line makes that line a comment.*</small>
