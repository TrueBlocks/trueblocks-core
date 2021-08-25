whenBlock argc: 2 [1:-th] 
whenBlock -th 
### Usage

`Usage:`    chifra when [-l|-t|-s|-v|-h] &lt; block | date &gt; [ block... | date... ]  
`Purpose:`  Find block(s) based on date, blockNum, timestamp, or 'special'.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | block_list | one or more dates, block numbers, hashes, or special named blocks (see notes) |
| -l | --list | export a list of the 'special' blocks |
| -t | --timestamps | ignore other options and generate timestamps only |
| -s | --skip &lt;num&gt; | only applicable if --timestamps is on, the step between block numbers in the export |
|####|Hidden options||
| -c | --check | available only with --timestamps option, checks the validity of the timestamp data |
| -f | --fix | available only with --timestamps option, fixes incorrect timestamps if found |
|####|Hidden options||
| -x | --fmt &lt;val&gt; | export format, one of *[ none \| json\* \| txt \| csv \| api ]* |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- The block list may contain any combination of `number`, `hash`, `date`, special `named` blocks.
- Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].

