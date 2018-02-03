makeClass argc: 2 [1:-th] 
makeClass -th 
#### Usage

`Usage:`    makeClass [-o|-r|-f|-l|-h|-c|-n|-s|-a|-v|-h] className  
`Purpose:`  Creates C++ code based on definition file at ./classDefinition/<className>.
             
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | className | name of C++ class(es) to process |
| -o | --open | edit <className(s)> definition file in local folder |
| -r | --run | run the class maker on associated <className(s)> |
| -f | --filter val | process only files with :filter in their names |
| -l | --list | list all definition files found in the local folder |
| -h | --header | write headers files only |
| -c | --source | write source files only |
| -n | --namespace val | surround the code with a --namespace:ns |
| -s | --silent | on error (no classDefinition file) exit silently |
| -a | --all | clear, edit, list, or run all class definitions found in the local folder |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

