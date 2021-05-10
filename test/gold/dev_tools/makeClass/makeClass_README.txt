makeClass argc: 2 [1:-th] 
makeClass -th 
### Usage

`Usage:`    makeClass [-r|-a|-j|-o|-m|-f|-l|-n|-i|-t|-c|-v|-h] &lt;file&gt; [file...]  
`Purpose:`  Automatically writes C++ for various purposes.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | files | one or more class definition files (required) |
| -r | --run | run the class maker on associated <class_name(s)> |
| -a | --all | list, or run all class definitions found in the local folder |
| -j | --js | export javaScript code from the class definition |
| -o | --options | export options code (check validity in the process) |
| -m | --readmes | create readme files for each tool and app |
| -f | --format | format source code files (.cpp and .h) found in local folder and below |
| -l | --lint | lint source code files (.cpp and .h) found in local folder and below |
| -n | --nspace &lt;str&gt; | surround generated c++ code with a namespace |
| -i | --filter &lt;str&gt; | process only files whose filename or contents contain 'filter' |
| -t | --test | for both code generation and options generation, process but do not write changes |
| -c | --force | for both code generation and options generation, force writing of changes |
|####|Hidden options||
| -e | --edit | edit <class_name(s)> definition file in local folder |
| -d | --dump | dump any classDefinition config tomls to screen and quit |
| -p | --api | generate api options file in explorer repo |
| -A | --openapi | export openapi.yaml file for API documentation |
|####|Hidden options||
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- The `--options` flag generates `COption` code for each of the various tools.
- The `--class` flag generates c++ code for each definition found in the local folder.
- More information on class definition files is found in the documentation.

