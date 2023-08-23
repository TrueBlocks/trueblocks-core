<!-- markdownlint-disable MD041 -->
`[{NAME}]` retrieves ABI files for the given address(es). It searches for the ABI in this order:
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
