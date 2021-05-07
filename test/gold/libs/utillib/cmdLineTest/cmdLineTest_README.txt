cmdLineTest argc: 2 [1:-th] 
cmdLineTest -th 
### Usage

`Usage:`     [-b|-i|-u|-s|-r|-l|-v|-h] testNum optionalMode  
`Purpose:`  Tests various command line behavior.
        
`Where:`  

| Hotkey | Option | Description |
| :----- | :----- | :---------- |
|  | testNum | the number of the test to run (required) |
|  | optionalMode | an optional mode with ~! start |
| -b | --bool <bool> | enter a boolean value (either '0', '1', 'false', or 'true') |
| -i | --int <int> | enter any numeric value |
| -u | --uint <uint> | enter any numeric value greater than or equal to zero |
| -s | --string <str> | enter any value |
| -r | --range <range> | enter a range of numeric values |
| -l | --list <list> | enter a list of value separated by commas (no spaces or quoted) |
|####|Hidden options||
| -d | --hiddenOption | a hidden option with an alternative hot key |
|####|Hidden options||
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

