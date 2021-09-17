## makeClass

`makeClass` is development-only tool that aids in various aspects of building TrueBlocks. The program is disabled for most users, but for core developers it can be used to automatically generate help files, API interface specs, command line options parsers and many of the test case files. Generally, regular users will not use `makeClass`.

### usage

`Usage:`    makeClass [-r|-a|-o|-m|-f|-l|-t|-n|-i|-c|-v|-h] &lt;file&gt; [file...]  
`Purpose:`  Automatically writes C++ for various purposes.

`Where:`

{{<td>}}
|     | Option               | Description                                                                   |
| --- | -------------------- | ----------------------------------------------------------------------------- |
|     | files                | one or more class definition files (required)                                 |
| -r  | --run                | run the class maker on associated <class_name(s)>                             |
| -a  | --all                | list, or run all class definitions found in the local<br/>folder              |
| -o  | --options            | export options code (check validity in the process)                           |
| -m  | --readmes            | create readme files for each tool and app                                     |
| -f  | --format             | format source code files (.cpp and .h) found in local<br/>folder and below    |
| -l  | --lint               | lint source code files (.cpp and .h) found in local<br/>foder and below       |
| -t  | --tsx                | create typescript routes                                                      |
| -n  | --nspace &lt;str&gt; | surround generated c++ code with a namespace                                  |
| -i  | --filter &lt;str&gt; | process only files whose filename or contents contain<br/>'filter'            |
| -c  | --force              | for both code generation and options generation, force<br/>writing of changes |
| -v  | --verbose            | set verbose level (optional level defaults to 1)                              |
| -h  | --help               | display this help screen                                                      |
{{</td>}}

`Notes:`

- The `--options` flag generates `COption` code for each of the various tools.
- More information on class definition files is found in the documentation.

**Source code**: [`dev_tools/makeClass`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/dev_tools/makeClass)

