makeClass argc: 2 [1:-th] 
makeClass -th 
#### Usage

`Usage:`    makeClass [-l|-r|-a|-j|-o|-n|-f|-t|-v|-h] &lt;file&gt; [file...]  
`Purpose:`  Automatically writes C++ for various purposes.

`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | files | one or more class definition files (required) |
| -l | --list | list all definition files found in the local ./classDefinitions folder |
| -r | --run | run the class maker on associated <class_name(s)> |
| -a | --all | list, or run all class definitions found in the local folder |
| -j | --js <str> | export javaScript code from the class definition |
| -o | --options | export options code (check validity in the process) |
| -n | --nspace <str> | surround generated c++ code with a namespace |
| -f | --filter <str> | process only files whose filename or contents contain 'filter' |
| -t | --test | for both code generation and options generation, process but do not write changes |

#### Hidden options (shown during testing only)
| -e | --edit | edit <class_name(s)> definition file in local folder |
#### Hidden options (shown during testing only)

| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- The `--options` flag generates `COption` code for each of the various tools.
- The `--class` flag generates c++ code for each definition found in the local folder.
- More information on class definition files is found in the documentation.

