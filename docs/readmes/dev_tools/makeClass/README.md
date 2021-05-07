## makeClass

`makeClass` is development-only tool that aids in various aspects of building TrueBlocks. The program is disabled for most users, but for core developers it can be used to automatically generate help files, API interface specs, command line options parsers and many of the test case files. Generally, regular users will not use `makeClass`.

### usage

`Usage:`    makeClass [-r|-a|-j|-o|-m|-f|-l|-n|-i|-t|-c|-v|-h] &lt;file&gt; [file...]  
`Purpose:`  Automatically writes C++ for various purposes.

`Where:`  

| Hotkey | Option | Description |
| :----- | :----- | :---------- |
|  | files | one or more class definition files (required) |
| -r | --run | run the class maker on associated <class_name(s)> |
| -a | --all | list, or run all class definitions found in the local folder |
| -j | --js | export javaScript code from the class definition |
| -o | --options | export options code (check validity in the process) |
| -m | --readmes | create readme files for each tool and app |
| -f | --format | format source code files (.cpp and .h) found in local folder and below |
| -l | --lint | lint source code files (.cpp and .h) found in local folder and below |
| -n | --nspace <str> | surround generated c++ code with a namespace |
| -i | --filter <str> | process only files whose filename or contents contain 'filter' |
| -t | --test | for both code generation and options generation, process but do not write changes |
| -c | --force | for both code generation and options generation, force writing of changes |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- The `--options` flag generates `COption` code for each of the various tools.
- The `--class` flag generates c++ code for each definition found in the local folder.
- More information on class definition files is found in the documentation.

**Source code**: [`dev_tools/makeClass`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/dev_tools/makeClass)

