## chifra abis

`chifra abis` retrieves ABI files for the given address(es). It searches for the ABI in this order: the current local folder, the TrueBlocks cache, [Etherscan](http://etherscan.io), or (in the future) ENS and Sourcify.

While this tool may be used from the command line and the API, its primary purpose is in support of tools such as `chifra export` to support the `--articulate` option.

The `--known` option prints a list of semi-standard function signatures such as the ERC20 standard, 721 standard, various functions from OpenZeppelin, various Uniswap functions, etc. As an optimization, the `known` signatures are searched first during articulation.

The `--sol` option converts the provided Solidity file into an ABI json file. The results are dropped into the current working folder.

The `--find` option is experimental. It scans the cross product of two sets. The first set contains more than 100,000 function and event names. The second set contains approximately 700 function signatures. The cross product of these two sets creates 70,000,000 combinations of `name(signature)` each of which is hashed to create either a four-byte or a 32-byte hash. Very infrequently, the tool will find matches for an otherwise unknown signatures.

```[plaintext]
Purpose:
  Fetches the ABI for a smart contract.

Usage:
  chifra abis [flags] <address> [address...]

Arguments:
  addrs - a list of one or more smart contracts whose ABIs to display (required)

Flags:
  -k, --known          load common 'known' ABIs from cache
  -s, --sol            extract the abi definition from the provided .sol file(s)
  -f, --find strings   search for function or event declarations given a four- or 32-byte code(s)
  -x, --fmt string     export format, one of [none|json*|txt|csv|api]
  -v, --verbose        enable verbose (increase detail with --log_level)
  -h, --help           display this help screen

Notes:
  - For the --sol option, place the solidity files in the current working folder.
  - Search for either four byte signatures or event signatures with the --find option.
```

#### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --wei             export values in wei (the default)
      --ether           export values in ether
      --dollars         export values in US dollars
      --raw             pass raw RPC data directly from the node with no processing
      --to_file         write the results to a temporary file and return the filename
      --output string   write the results to file 'fn' and return the filename
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
```

*For the `--file string` option, you may place a series of valid command lines in a file using any valid flags. In some cases, this may significantly improve performance. A semi-colon at the start of any line makes it a comment.*
