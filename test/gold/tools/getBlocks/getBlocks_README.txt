getBlocks argc: 2 [1:-th] 
getBlocks -th 
### Usage

`Usage:`    chifra blocks [-e|-U|-a|-u|-n|-c|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Returns block(s) from local cache or directly from a running node.

`Where:`  

| Hotkey | Option | Description |
| :----- | :----- | :---------- |
|  | blocks | a space-separated list of one or more blocks to retrieve (required) |
| -e | --hashes | display only transaction hashes, default is to display full transaction detail |
| -U | --uncles | display uncle blocks (if any) instead of the requested block |
| -a | --apps | display all address appearances in the block |
| -u | --uniq | display only uniq addresses appearances in the block |
| -n | --uniq_tx | display only uniq addresses per transaction in the block |
| -c | --count | display counts of appearances (for --apps, --uniq, or --uniq_tx) or transactions |
|####|Hidden options||
| -t | --trace | export the traces from the block as opposed to the block data |
| -o | --cache | force a write of the block to the cache |
|####|Hidden options||
| -x | --fmt <val> | export format, one of *[ none \| json\* \| txt \| csv \| api ]* |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- `blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.
- `blocks` may be specified as either numbers or hashes.
- `special` blocks are detailed under `chifra when --list`.

