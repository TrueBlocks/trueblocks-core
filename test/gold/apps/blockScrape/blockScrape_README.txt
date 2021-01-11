blockScrape argc: 2 [1:-th] 
blockScrape -th 
#### Usage

`Usage:`    blockScrape [-t|-n|-p|-u|-v|-h] mode  
`Purpose:`  Decentralized blockchain scraper and block cache.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | mode | control the block and account scrapers, one or more of [run*&#124;quit&#124;pause&#124;restart] |
| -t | --tool <val> | process the index, monitors, or both (none means process timestamps only), one or more of [monitors&#124;index*&#124;none&#124;both] |
| -n | --n_blocks <num> | maximum number of blocks to process (defaults to 5000) |
| -p | --pin | pin new chunks (and blooms) to IPFS (requires Pinata key) |
| -u | --publish | publish the hash of the pin manifest to the UnchainedIndex smart contract |

#### Hidden options (shown during testing only)
| -b | --n_block_procs <num> | number of block channels for blaze |
| -a | --n_addr_procs <num> | number of address channels for blaze |
| -l | --listpins | list pins (precludes other options, requires API key) |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

