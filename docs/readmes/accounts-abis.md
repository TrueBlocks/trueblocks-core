## chifra abis

<!-- markdownlint-disable MD041 -->
`chifra abis` retrieves ABI files for the given address(es). It searches for the ABI in this order:
the current local folder, the TrueBlocks cache, Etherscan, or (in the
future) ENS and Sourcify.

While this tool may be used from the command line and the API, its primary purpose is in support of
tools such as `chifra export` to support the `--articulate` option.

The `--known` option prints a list of semi-standard function signatures such as the ERC20 standard,
ERC 721 standard, various functions from OpenZeppelin, various Uniswap functions, etc. As an
optimization, the `known` signatures are searched first during articulation.

The `--sol` option converts the provided Solidity file into an ABI json file. The results are
dropped into the current working folder.

The `--find` option is experimental. Please see the notes below for more information.

```[plaintext]
Purpose:
  Fetches the ABI for a smart contract.

Usage:
  chifra abis [flags] <address> [address...]

Arguments:
  addrs - a list of one or more smart contracts whose ABIs to display (required)

Flags:
  -k, --known           load common 'known' ABIs from cache
  -f, --find strings    search for function or event declarations given a four- or 32-byte code(s)
  -n, --hint strings    for the --find option only, provide hints to speed up the search
  -e, --encode string   generate the 32-byte encoding for a given cannonical function or event signature
  -o, --cache           force the results of the query into the cache
  -D, --decache         removes related items from the cache
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose output
  -h, --help            display this help screen

Notes:
  - Search for either four byte signatures or event signatures with the --find option.
```

Data models produced by this tool:

- [abi](/data-model/accounts/#abi)
- [function](/data-model/other/#function)
- [parameter](/data-model/other/#parameter)

Links:

- [api docs](/api/#operation/accounts-abis)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/abis)

<!-- markdownlint-disable MD041 -->
### notes

Without the --verbose option, the result is a compacted form of the ABI. Add --verbose for full details.

The `chifra abis --find` option scans the cross product of two sets. The first set contains more than 100,000 function and event
names. The second set contains approximately 700 function signatures. The cross product of these two sets creates 70,000,000
combinations of name(signature) each of which is hashed to create either a four-byte or a 32-byte hash. Very infrequently,
the tool will find matches for an otherwise unknown signatures.

