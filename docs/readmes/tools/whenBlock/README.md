## chifra when

The `chifra when` tool answers one of two questions: (1) "At what date and time did a given block occur?" or (2) "What block occurred at or before a given date and time?"

In the first case, supply a block number or hash and the date and time of that block are displayed. In the later case, supply a date (and optionally a time) and the block number that occurred at or just prior to that date is displayed.

The values for `date` and `time` are specified in JSON format. `hour`/`minute`/`second` are optional, and if omitted, default to zero in each case. Block numbers may be specified as either integers or hexidecimal number or block hashes. You may specify any number of dates and/or blocks per invocation.

### usage

`Usage:`    chifra when [-l|-t|-v|-h] &lt; block | date &gt; [ block... | date... ]
`Purpose:`  Find block(s) based on date, blockNum, timestamp, or 'special'.

`Where:`

{{<td>}}
|     | Option            | Description                                                                       |
| --- | ----------------- | --------------------------------------------------------------------------------- |
|     | block_list        | one or more dates, block numbers, hashes, or special<br/>named blocks (see notes) |
| -l  | --list            | export a list of the 'special' blocks                                             |
| -t  | --timestamps      | ignore other options and generate timestamps only                                 |
| -x  | --fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                                 |
| -v  | --verbose         | set verbose level (optional level defaults to 1)                                  |
| -h  | --help            | display this help screen                                                          |
{{</td>}}

`Notes:`

- The block list may contain any combination of `number`, `hash`, `date`, special `named` blocks.
- Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].

**Source code**: [`tools/whenBlock`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/whenBlock)

