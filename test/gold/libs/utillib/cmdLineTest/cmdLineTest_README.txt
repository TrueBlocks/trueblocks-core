cmdLineTest argc: 2 [1:-th] 
cmdLineTest -th 
#### Usage

`Usage:`    cmdLineTest [-b|-i|-u|-s|-r|-l|-v|-h] testNum  
`Purpose:`  Tests various command line behavior.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | testNum | the number of the test to run |
| -b | --bool val | enter a boolean value (either `0`, `1`, `false`, or `true`) |
| -i | --int val | enter any numeric value |
| -u | --uint val | enter any numeric value greater than or equal to zero |
| -s | --string val | enter any value |
| -r | --range range | enter a range of numeric values |
| -l | --list list | enter a list of value separated by commas (no spaces or quoted) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

