blockScrape argc: 2 [1:--readme] 
blockScrape --readme 
### Usage

`Usage:`    chifra scrape [-p|-s|-v|-h]  
`Purpose:`  Scan the chain and update the TrueBlocks index of appearances.

`Where:`

{{<td>}}
|          | Option                             | Description                                                                         |
| -------- | ---------------------------------- | ----------------------------------------------------------------------------------- |
| &#8208;p | &#8208;&#8208;pin                  | pin new chunks (and blooms) to IPFS (requires Pinata<br/>key and running IPFS node) |
| &#8208;s | &#8208;&#8208;sleep &lt;double&gt; | the number of seconds to sleep between passes (default<br/>14)                      |
| &#8208;v | &#8208;&#8208;verbose              | set verbose level (optional level defaults to 1)                                    |
| &#8208;h | &#8208;&#8208;help                 | display this help screen                                                            |
{{</td>}}

`Configurable Items:`

- `n_blocks`: maximum number of blocks to process (defaults to 5000).
- `n_block_procs`: number of concurrent block channels for blaze.
- `n_addr_procs`: number of concurrent address channels for blaze.

