getBlocks argc: 2 [1:--readme] 
getBlocks --readme 
### Usage

`Usage:`    chifra blocks [-e|-U|-t|-a|-u|-n|-c|-o|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Retrieve one or more blocks from the chain or local cache.

`Where:`

{{<td>}}
|          | Option                               | Description                                                                         |
| -------- | ------------------------------------ | ----------------------------------------------------------------------------------- |
|          | blocks                               | a space-separated list of one or more block identifiers<br/>(required)              |
| &#8208;e | &#8208;&#8208;hashes                 | display only transaction hashes, default is to display<br/>full transaction detail  |
| &#8208;U | &#8208;&#8208;uncles                 | display uncle blocks (if any) instead of the requested<br/>block                    |
| &#8208;t | &#8208;&#8208;trace                  | export the traces from the block as opposed to the<br/>block data                   |
| &#8208;a | &#8208;&#8208;apps                   | display only the list of address appearances in the<br/>block                       |
| &#8208;u | &#8208;&#8208;uniq                   | display only the list of uniq address appearances in<br/>the block                  |
| &#8208;n | &#8208;&#8208;uniq_tx                | display only the list of uniq address appearances in<br/>each transaction           |
| &#8208;c | &#8208;&#8208;count                  | display the number of the lists of appearances for<br/>--apps, --uniq, or --uniq_tx |
| &#8208;o | &#8208;&#8208;cache                  | force a write of the block to the cache                                             |
| ###      | Hidden options                       |                                                                                     |
| &#8208;l | &#8208;&#8208;list &lt;num&gt;       | summary list of blocks running backwards from latest<br/>block minus num            |
| &#8208;C | &#8208;&#8208;list_count &lt;num&gt; | the number of blocks to report for --list option                                    |
| ###      | Hidden options                       |                                                                                     |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;        | export format, one of [none, json, txt, csv, api]                                   |
| &#8208;v | &#8208;&#8208;verbose                | set verbose level (optional level defaults to 1)                                    |
| &#8208;h | &#8208;&#8208;help                   | display this help screen                                                            |
{{</td>}}

`Notes:`

- `blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.
- `blocks` may be specified as either numbers or hashes.
- `special` blocks are detailed under `chifra when --list`.

