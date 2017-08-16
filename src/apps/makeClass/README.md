## makeClass

`makeClass` is the heart of [chifra](../chifra). It builds C++ class and header files from a classDefinition.txt file.
#### Usage

`Usage:`    makeClass [-e|-f|-l|-n|-s|-r|-a|-v|-h] className  
`Purpose:`  Creates a C++ class based on definitions found in ./classDefinition/<className>.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | className | name of C++ class(es) to process |
| -e | --edit | edit <className(s)> definition file in local folder |
| -f | --filter | process only files with :filter in their names |
| -l | --list | list all definition files found in the local folder |
| -n | --namespace | surround the code with a --namespace:ns |
| -s | --silent | on error (no classDefinition file) exit silently |
| -r | --run | run the class maker on associated <className(s)> |
| -a | --all | clear, edit, list, or run all class definitions found in the local folder |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All QuickBlocks command-line tools support the following commands (although in some case, they have no meaning):

    Command     |         Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolors  |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.
*For the `--file:fn` option, place a series of valid command lines in a file and use the above option. In some cases, this option may significantly improve performance. Use semi-colon make comments.*

#### Powered by QuickBlocks&reg;

