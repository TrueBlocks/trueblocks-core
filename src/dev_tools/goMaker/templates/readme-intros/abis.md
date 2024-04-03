The `chifra {{.Route}}` tool retrieves one or more ABI files for the given address(es). It searches
for ABIs, sequentially, in the following locations:

- the current working folder,
- the TrueBlocks local cache,
- Etherscan,
- (in the future) ENS and Sourcify.

While this tool may be used from the command line, its primary purpose is in support of
the `--articulate` option for tools such as `chifra export` and `chifra logs`.

If possible, the tool will follow proxied addresses searching for the ABI, but that does not
always work. In that case, you may use the `--proxy_for` option.

The `--known` option prints a list of semi-standard function signatures such as the ERC20 standard,
ERC 721 standard, various functions from OpenZeppelin, various Uniswap functions, etc. As an
optimization, the `known` signatures are searched first during articulation.

The `--encode` option generates a 32-byte encoding for a given cannonical function or event signature. For
functions, you may manually extract the first four bytes of the hash.

The `--find` option is experimental. Please see the notes below for more information.
