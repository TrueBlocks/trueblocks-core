blockScrape argc: 2 [1:-th] 
blockScrape -th 
### Usage

`Usage:`    chifra scrape [-t|-n|-p|-s|-v|-h] mode  
`Purpose:`  Scan the chain and update the TrueBlocks index of appearances.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | mode | control the block and account scrapers, one of *[ run \| quit \| pause \| restart ]* (required) |
| -t | --tool &lt;val&gt; | process the index, monitors, or both (none means process timestamps only), one or more of *[ monitors \| index\* \| none \| both ]* |
| -n | --n_blocks &lt;num&gt; | maximum number of blocks to process (defaults to 5000) |
| -p | --pin | pin new chunks (and blooms) to IPFS (requires Pinata key and running IPFS node) |
| -s | --sleep &lt;double&gt; | the number of seconds to sleep between passes (default 14) |
|####|Hidden options||
| -b | --n_block_procs &lt;num&gt; | number of block channels for blaze |
| -a | --n_addr_procs &lt;num&gt; | number of address channels for blaze |
| -s | --staging | pass through to chifra export --freshen (see notes) |
| -u | --unripe | pass through to chifra export --freshen (see notes) |
| -i | --cache_txs | pass through to chifra export --freshen (see notes) |
| -R | --cache_traces | pass through to chifra export --freshen (see notes) |
|  | --load &lt;str&gt; | pass through to chifra export --freshen (see notes) |
|####|Hidden options||
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- Certain options (`--cache_txs`, `--cache_traces`, etc.) are passed through to `chifra export` 
  if `tool` includes `monitors`. See `chifra export --help`.

