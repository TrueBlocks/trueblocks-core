## chifra when

The `chifra when` tool answers one of two questions: (1) "At what date and time did a given block occur?" or (2) "What block occurred at or before a given date and time?"

In the first case, supply a block number or hash and the date and time of that block are displayed. In the later case, supply a date (and optionally a time) and the block number that occurred at or just prior to that date is displayed.

The values for `date` and `time` are specified in JSON format. `hour`/`minute`/`second` are optional, and if omitted, default to zero in each case. Block numbers may be specified as either integers or hexidecimal number or block hashes. You may specify any number of dates and/or blocks per invocation.

### Usage

`Usage:`    chifra when [-l|-t|-v|-h] &lt; block | date &gt; [ block... | date... ]  
`Purpose:`  Find block(s) based on date, blockNum, timestamp, or 'special'.

`Where:`

|          | Option                        | Description                                                                       |
| -------- | ----------------------------- | --------------------------------------------------------------------------------- |
|          | blocks                        | one or more dates, block numbers, hashes, or special<br/>named blocks (see notes) |
| &#8208;l | &#8208;&#8208;list            | export a list of the 'special' blocks                                             |
| &#8208;t | &#8208;&#8208;timestamps      | ignore other options and generate timestamps only                                 |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                                 |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                                  |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                                          |

`Notes:`

- The block list may contain any combination of `number`, `hash`, `date`, special `named` blocks.
- Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].

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
| --very_raw  | report JSON data from node with zero processing               |
| --to_file   | write the results to a temporary file and return the filename |
| --output:fn | write the results to file 'fn' and return the filename        |
| --file:fn   | specify multiple sets of command line options in a file.      |

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source code**: [`tools/whenBlock`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/whenBlock)

