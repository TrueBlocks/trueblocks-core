blockScrape argc: 2 [1:-th] 
blockScrape -th 
### Usage

`Usage:`    chifra scrape [-p|-s|-v|-h]  
`Purpose:`  Scan the chain and update the TrueBlocks index of appearances.

`Where:`  

|     | Option                 | Description                                                                     |
| --- | ---------------------- | ------------------------------------------------------------------------------- |
| -p  | --pin                  | pin new chunks (and blooms) to IPFS (requires Pinata key and running IPFS node) |
| -s  | --sleep &lt;double&gt; | the number of seconds to sleep between passes (default 14)                      |
| -v  | --verbose              | set verbose level (optional level defaults to 1)                                |
| -h  | --help                 | display this help screen                                                        |

`Configurable Items:`

`n_blocks`: maximum number of blocks to process (defaults to 5000).
`n_block_procs`: number of concurrent block channels for blaze.
`n_addr_procs`: number of concurrent address channels for blaze.

