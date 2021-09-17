## chifra blocks

The `chifra blocks` tool retrieves block data from your Ethereum node or, if previously cached, from the TrueBlocks cache. You may specify multiple blocks per invocation.

By default, `chifra blocks` queries the full transactional details of the block (including receipts). You may optionally retreive only the transaction hashes in the block (which is signifcantly faster). Additionally, you may also use this tool to retrieve uncle blocks at a give height.

Another useful feature of `chifra blocks` is the ability to extract address appearances from a block. TrueBlocks uses a similar feature internally to build its index of appearances. This type of data is very insightful when studying end user behaviour and chain-wide adoption analysis.

### Usage

`Usage:`    chifra blocks [-e|-U|-t|-a|-u|-n|-c|-o|-v|-h] &lt;block&gt; [block...]  
`Purpose:`  Retrieve one or more blocks from the chain or local cache.

`Where:`  

|     | Option            | Description                                                                     |
| --- | ----------------- | ------------------------------------------------------------------------------- |
|     | blocks            | a space-separated list of one or more block identifiers (required)              |
| -e  | --hashes          | display only transaction hashes, default is to display full transaction detail  |
| -U  | --uncles          | display uncle blocks (if any) instead of the requested block                    |
| -t  | --trace           | export the traces from the block as opposed to the block data                   |
| -a  | --apps            | display only the list of address appearances in the block                       |
| -u  | --uniq            | display only the list of uniq address appearances in the block                  |
| -n  | --uniq_tx         | display only the list of uniq address appearances in each transaction           |
| -c  | --count           | display the number of the lists of appearances for --apps, --uniq, or --uniq_tx |
| -o  | --cache           | force a write of the block to the cache                                         |
| -x  | --fmt &lt;val&gt; | export format, one of *[ none \| json\* \| txt \| csv \| api ]*                 |
| -v  | --verbose         | set verbose level (optional level defaults to 1)                                |
| -h  | --help            | display this help screen                                                        |

`Notes:`

- `blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.
- `blocks` may be specified as either numbers or hashes.
- `special` blocks are detailed under `chifra when --list`.

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

| Command     | Description                                                                                     |
| ----------- | ----------------------------------------------------------------------------------------------- |
| --version   | display the current version of the tool                                                         |
| --nocolor   | turn off colored display                                                                        |
| --wei       | specify value in wei (the default)                                                              |
| --ether     | specify value in ether                                                                          |
| --dollars   | specify value in US dollars                                                                     |
| --raw       | report JSON data from the node with minimal processing                                          |
| --very_raw  | report JSON data from node with zero processing                                                 |
| --fmt       | export format (where appropriate). One of [ none &#124; txt &#124; csv &#124; json &#124; api ] |
| --to_file   | write the results to a temporary file and return the filename                                   |
| --output:fn | write the results to file 'fn' and return the filename                                          |
| --file:fn   | specify multiple sets of command line options in a file.                                        |

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source code**: [`tools/getBlocks`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/getBlocks)

