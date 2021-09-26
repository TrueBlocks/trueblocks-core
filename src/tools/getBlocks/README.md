## chifra blocks

The `chifra blocks` tool retrieves block data from your Ethereum node or, if previously cached, from the TrueBlocks cache. You may specify multiple blocks per invocation.

By default, `chifra blocks` queries the full transactional details of the block (including receipts). You may optionally retreive only the transaction hashes in the block (which is signifcantly faster). Additionally, you may also use this tool to retrieve uncle blocks at a give height.

Another useful feature of `chifra blocks` is the ability to extract address appearances from a block. TrueBlocks uses a similar feature internally to build its index of appearances. This type of data is very insightful when studying end user behaviour and chain-wide adoption analysis.

### Usage

`Usage:`    chifra blocks [-e|-U|-t|-a|-u|-n|-c|-o|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Retrieve one or more blocks from the chain or local cache.

`Where:`

|          | Option                        | Description                                                                         |
| -------- | ----------------------------- | ----------------------------------------------------------------------------------- |
|          | blocks                        | a space-separated list of one or more block identifiers<br/>(required)              |
| &#8208;e | &#8208;&#8208;hashes          | display only transaction hashes, default is to display<br/>full transaction detail  |
| &#8208;U | &#8208;&#8208;uncles          | display uncle blocks (if any) instead of the requested<br/>block                    |
| &#8208;t | &#8208;&#8208;trace           | export the traces from the block as opposed to the<br/>block data                   |
| &#8208;a | &#8208;&#8208;apps            | display only the list of address appearances in the<br/>block                       |
| &#8208;u | &#8208;&#8208;uniq            | display only the list of uniq address appearances in<br/>the block                  |
| &#8208;n | &#8208;&#8208;uniq_tx         | display only the list of uniq address appearances in<br/>each transaction           |
| &#8208;c | &#8208;&#8208;count           | display the number of the lists of appearances for<br/>--apps, --uniq, or --uniq_tx |
| &#8208;o | &#8208;&#8208;cache           | force a write of the block to the cache                                             |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                                   |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                                    |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                                            |

`Notes:`

- `blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.
- `blocks` may be specified as either numbers or hashes.
- `special` blocks are detailed under `chifra when --list`.

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

| Command     | Description                                                   |
| ----------- | ------------------------------------------------------------- |
| --version   | display the current version of the tool                       |
| --nocolor   | turn off colored display                                      |
| --wei       | specify value in wei (the default)                            |
| --ether     | specify value in ether                                        |
| --dollars   | specify value in US dollars                                   |
| --raw       | report JSON data from the node with minimal processing        |
| --to_file   | write the results to a temporary file and return the filename |
| --output:fn | write the results to file 'fn' and return the filename        |
| --file:fn   | specify multiple sets of command line options in a file       |

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source code**: [`tools/getBlocks`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getBlocks)

