## chifra abis

`chifra abis` retrieves a smart contract's ABI file either from the current local folder, from the TrueBlocks cache, from [Etherscan](http://etherscan.io), or (in the future) from ENS and Sourcify.

While this tool may be used from the command line or the API, its primary use is in support of the `--articulate` option of other tools such as `chifra export`.

The `--known` option prints a list of standard and semi-standard function signatures such as the ERC20 standard, various OpenZeppelin functions, the base and pair Uniswap functions, etc.

The `--find` option is experimental. It scans a cross of approx. 100,000 function names with approx. 700 function signatures, generating at each cross a four byte signature and looking for a match to the provided fourbyte. Very infrequently, this tool will find a match for an otherwise unknown fourbyte string.

The `--sol` option will convert a single Solidity file found in the current folder into an ABI.

```[plaintext]
Purpose:
  Fetches the ABI for a smart contract.

Usage:
  chifra abis [flags] <address> [address...]

Arguments:
  addrs - a list of one or more smart contracts whose ABIs to display (required)

Flags:
  -k, --known          load common 'known' ABIs from cache
  -s, --sol strings    extract the abi definition from the provided .sol file(s)
  -f, --find strings   search for function or event declarations given a four- or 32-byte code(s)

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - For the --sol option, place the solidity files in the current working folder.
  - Search for either four byte signatures or event signatures with the --find option.
```

**Source code**: [`tools/grabABI`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/grabABI)

