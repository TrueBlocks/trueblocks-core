whenBlock argc: 2 [1:-th] 
whenBlock -th 
### Usage

`Usage:`    chifra when [-l|-t|-v|-h] &lt; block | date &gt; [ block... | date... ]  
`Purpose:`  Find block(s) based on date, blockNum, timestamp, or 'special'.

`Where:`

{{<td>}}
|          | Option                         | Description                                                                             |
| -------- | ------------------------------ | --------------------------------------------------------------------------------------- |
|          | block_list                     | one or more dates, block numbers, hashes, or special<br/>named blocks (see notes)       |
| &#8208;l | &#8208;&#8208;list             | export a list of the 'special' blocks                                                   |
| &#8208;t | &#8208;&#8208;timestamps       | ignore other options and generate timestamps only                                       |
| ###      | Hidden options                 |                                                                                         |
| &#8208;c | &#8208;&#8208;check            | available only with --timestamps, checks the validity<br/>of the timestamp data         |
| &#8208;f | &#8208;&#8208;fix              | available only with --timestamps, fixes incorrect timestamps<br/>if any                 |
| &#8208;u | &#8208;&#8208;count            | available only with --timestamps, returns the number<br/>of timestamps in the cache     |
| &#8208;s | &#8208;&#8208;skip &lt;num&gt; | only applicable if --timestamps is on, the step between<br/>block numbers in the export |
| ###      | Hidden options                 |                                                                                         |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;  | export format, one of [none, json, txt, csv, api]                                       |
| &#8208;v | &#8208;&#8208;verbose          | set verbose level (optional level defaults to 1)                                        |
| &#8208;h | &#8208;&#8208;help             | display this help screen                                                                |
{{</td>}}

`Notes:`

- The block list may contain any combination of `number`, `hash`, `date`, special `named` blocks.
- Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].

