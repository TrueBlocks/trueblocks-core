## chifra init

When invoked, `chifra init` looks at a smart contract called **The Unchained Index** ([0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd](https://etherscan.io/address/0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd)). From this smart contract, it extracts a data item called `manifestHash`. The `manifestHash` is an IPFS hash that points to a file (a manifest) that contains every previously pinned bloom filter and index chunk. TrueBlocks periodically publishes the manifest's hash to the smart contract. This makes the entire index both available for our software to use and impossible for us to withhold. Both of these aspects of the manifest are included by design.

If you stop `chifra init` before it finishes, it will pick up against where it left off the next time you run it.

Certain parts of the system (`chifra list` and `chifra export` for example) if you have not previously run `chifra init` or `chifra scrape`. You will be warned by the system until it's satified.

If you run `chifra init` and allow it to complete, the next time you run `chifra scrape`, it will start where `init` finished. This means that only the blooms will be stored on your hard drive. Subsequent scraping will produce both chunks and blooms, although you can, if you wish delete chunks that are not being used. You may periodically run `chifra init` if you prefer not to scrape.

### usage

`Usage:`    chifra init  
`Purpose:`  Leech the bloom filters from IPFS by first downloading the pin manifest from a smart contract and then downloading the blooms. Optionally `--pin` the resulting download in order to share it with others.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
| -i | --init | initialize local index by downloading bloom filters from pinning service |
| -k | --init_all | initialize local index by downloading both bloom filters and index chunks |
| -p | --pin_locally | pin all local files in the index to an IPFS store (requires IPFS) |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- One of `--list`, `--init`, or `--init_all` is required.
- the `--pin_locally` option only works if the IPFS executable is in your path.
**Source code**: [`apps/pinMan`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/pinMan)
