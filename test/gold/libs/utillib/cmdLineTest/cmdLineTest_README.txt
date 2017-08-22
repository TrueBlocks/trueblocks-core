cmdLineTest argc: 2 [1:-th] 
cmdLineTest -th 
#### Usage

`Usage:`    cmdLineTest [-b|-i|-u|-s|-r|-l|-v|-h] testNum  
`Purpose:`  Tests various command line behaviour.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | testNum | the number of the test to run |
| -b | --bool value | enter a boolean value (either `0`, `1`, `false`, or `true`) |
| -i | --int value | enter any numeric value |
| -u | --uint value | enter any any numeric value greater than or equal to zero |
| -s | --string value | enter any value |
| -r | --range start-end | enter a range of numeric values |
| -l | --list item1,item2,... | enter a list of value separated by commas (no spaces or quoted) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

