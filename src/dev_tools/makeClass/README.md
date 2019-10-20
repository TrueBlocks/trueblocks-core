## makeClass

`makeClass` is the heart of [chifra](../chifra). It builds C++ class and header files from a classDefinition files. Class definitions files are placed in a folder called classDefinition in the folder one wishes the resulting C++ code to appear.

The format of the classDefinition file is described below.

#### Usage

`Usage:`    makeClass [-o|-r|-j|-f|-l|-h|-c|-n|-s|-a|-v|-h] className  
`Purpose:`  Creates C++ code based on definition file at ./classDefinition/<className>.
        
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | className | name of C++ class(es) to process (required) |
| -o | --open | edit <className(s)> definition file in local folder |
| -r | --run | run the class maker on associated <className(s)> |
| -j | --js <class> | export javaScript components for 'class' |
| -f | --filter <string> | process only files with :filter in their names |
| -l | --list | list all definition files found in the local folder |
| -h | --header | write headers files only |
| -c | --source | write source files only |
| -n | --namespace <string> | surround the code with a --namespace:ns |
| -s | --silent | on error (no classDefinition file) exit silently |
| -a | --all | clear, edit, list, or run all class definitions found in the local folder |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All **QBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --raw       |   report JSON data from the node with minimal processing
    --very_raw  |   report JSON data from node with zero processing
    --fmt       |   export format (where appropriate). One of [none|txt|csv|json|api]
    --to_file   |   write the results to a temporary file and return the filename
    --output:fn |   write the results to file 'fn' and return the filename
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**


