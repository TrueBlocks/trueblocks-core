## chifra abis

`chifra abis` retrieves an ABI file either from a local cache, from the Ethereum Name Service (ENS), or from [Etherscan](http://etherscan.io). Once retrieved, `chifra abis` generates the classDefinition files needed by [makeClass](../makeClass/README.md) to generate a parselib for each [Ethereum address monitor](../../monitors/README.md).

### Usage

`Usage:`    chifra abis [-c|-k|-f|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Fetches the ABI for a smart contract.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | addrs | list of one or more smart contracts whose ABI to grab from EtherScan (required) |
| -c | --canonical | convert all types to their canonical represenation and remove all spaces from display |
| -k | --known | load common 'known' ABIs from cache |
| -f | --find <str> | try to search for a function declaration given a four byte code |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- Solidity files found in the local folder with the name '<address>.sol' are converted to an ABI prior to processing (and then removed).

**Source**: [`tools/grabABI`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/grabABI)

