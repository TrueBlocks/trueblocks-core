cmdLineTest argc: 2 [1:-th] 
cmdLineTest -th 
### Usage

`Usage:`     [-b|-i|-u|-s|-r|-l|-v|-h] testNum optionalMode  
`Purpose:`  Tests various command line behavior.
        
`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | testNum | the number of the test to run (required) |
|  | optionalMode | an optional mode with ~! start |
| -b | --bool &lt;bool&gt; | enter a boolean value (either '0', '1', 'false', or 'true') |
| -i | --int &lt;int&gt; | enter any numeric value |
| -u | --uint &lt;uint&gt; | enter any numeric value greater than or equal to zero |
| -s | --string &lt;str&gt; | enter any value |
| -r | --range &lt;range&gt; | enter a range of numeric values |
| -l | --list &lt;list&gt; | enter a list of value separated by commas (no spaces or quoted) |
|####|Hidden options||
| -d | --hiddenOption | a hidden option with an alternative hot key |
|####|Hidden options||
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

