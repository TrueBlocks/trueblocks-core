chifra argc: 2 [1:-th] 
chifra -th 
#### Usage

`Usage:`    chifra [-s|-v|-h] command  
`Purpose:`  Create a TrueBlocks monitor configuration.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | command | which command to run, one or more of [list&#124;export&#124;slurp&#124;accounts&#124;abi&#124;state&#124;tokens&#124;data&#124;blocks&#124;transactions&#124;receipts&#124;logs&#124;traces&#124;quotes&#124;scrape&#124;status&#124;config&#124;rm&#124;message&#124;leech&#124;seed] (required) |
| -s | --sleep <num> | for the 'scrape' and 'daemon' commands, the number of seconds chifra should sleep between runs (default 14) |

#### Hidden options (shown during testing only)
| -e | --set | for status config only, indicates that this is config --sef |
| -t | --tool_help | call into the underlying tool's help screen |
|   | --fmt <val> | export format, one of [none&#124;json*&#124;txt&#124;csv&#124;api] |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

