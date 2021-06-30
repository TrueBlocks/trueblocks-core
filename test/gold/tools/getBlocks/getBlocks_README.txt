getBlocks argc: 2 [1:-th] 
getBlocks -th 
### Usage

`Usage:`    chifra blocks [-e|-U|-t|-a|-u|-n|-c|-o|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Retrieve one or more blocks from the chain or local cache.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | blocks | a space-separated list of one or more blocks to retrieve (required) |
| -e | --hashes | display only transaction hashes, default is to display full transaction detail |
| -U | --uncles | display uncle blocks (if any) instead of the requested block |
| -t | --trace | export the traces from the block as opposed to the block data |
| -a | --apps | display all address appearances in the block |
| -u | --uniq | display only uniq address appearances per block |
| -n | --uniq_tx | display only uniq address appearances per transaction |
| -c | --count | display counts of appearances (for --apps, --uniq, or --uniq_tx) or transactions |
| -o | --cache | force a write of the block to the cache |
|####|Hidden options||
| -l | --list &lt;num&gt; | summary list of blocks running backwards from latest block minus num |
| -C | --list_count &lt;num&gt; | the number of blocks to report for --list option |
|####|Hidden options||
| -x | --fmt &lt;val&gt; | export format, one of *[ none \| json\* \| txt \| csv \| api ]* |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- `blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.
- `blocks` may be specified as either numbers or hashes.
- `special` blocks are detailed under `chifra when --list`.

