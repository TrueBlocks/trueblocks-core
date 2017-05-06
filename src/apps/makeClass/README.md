## makeClass


#### Help Screen

`Usage:`    makeClass [-c|-e|-l|-f|-r|-a|-v|-h] className  
`Purpose:`  Creates a C++ class based on definitions found in ./classDefinition/<className>.
             
`Where:`  

| Option | Full Command | Description |
| -------: | :------- | :------- |
|  | className | name of C++ class(es) to process |
| -c | --clear | remove <className(s)> definition file from local folder |
| -e | --edit | edit <className(s)> definition file in local folder |
| -l | --list | list all definition files found in the local folder |
| -f | --filter | process only files with :filter in their names |
| -r | --run | run the class maker on associated <className(s)> |
| -a | --all | clear, edit, list, or run all class definitions found in the local folder |
| -v | --verbose | set verbose level. Follow with a number to set level (-v0 for silent) |
| -h | --help | display this help screen |

##### The --file option

All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate lines, in a file and include the above option. In some cases, this option may significantly improve performance. Please a semi-colon at the start of a line to make it a comment.

Powered by QuickBlocks.io
