## makeClass

`makeClass` is development-only tool that aids in various aspects of building TrueBlocks. The program is disabled for most users, but for core developers it can be used to automatically generate help files, API interface specs, command line options parsers and many of the test case files. Generally, regular users will not use `makeClass`.

### Usage

`Usage:`    makeClass [-r|-a|-o|-m|-f|-l|-n|-i|-c|-v|-h] &lt;file&gt; [file...]  
`Purpose:`  Automatically writes C++ for various purposes.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | files | one or more class definition files (required) |
| -r | --run | run the class maker on associated <class_name(s)> |
| -a | --all | list, or run all class definitions found in the local folder |
| -o | --options | export options code (check validity in the process) |
| -m | --readmes | create readme files for each tool and app |
| -f | --format | format source code files (.cpp and .h) found in local folder and below |
| -l | --lint | lint source code files (.cpp and .h) found in local folder and below |
| -n | --nspace &lt;str&gt; | surround generated c++ code with a namespace |
| -i | --filter &lt;str&gt; | process only files whose filename or contents contain 'filter' |
| -c | --force | for both code generation and options generation, force writing of changes |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- The `--options` flag generates `COption` code for each of the various tools.
- More information on class definition files is found in the documentation.

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

| Command     | Description                                                                                     |
| ----------- | ----------------------------------------------------------------------------------------------- |
| --version   | display the current version of the tool                                                         |
| --nocolor   | turn off colored display                                                                        |
| --wei       | specify value in wei (the default)                                                              |
| --ether     | specify value in ether                                                                          |
| --dollars   | specify value in US dollars                                                                     |
| --raw       | report JSON data from the node with minimal processing                                          |
| --very_raw  | report JSON data from node with zero processing                                                 |
| --fmt       | export format (where appropriate). One of [ none &#124; txt &#124; csv &#124; json &#124; api ] |
| --to_file   | write the results to a temporary file and return the filename                                   |
| --output:fn | write the results to file 'fn' and return the filename                                          |
| --file:fn   | specify multiple sets of command line options in a file.                                        |

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source code**: [`dev_tools/makeClass`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/dev_tools/makeClass)

