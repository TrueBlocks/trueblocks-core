## makeClass

`makeClass` is development-only tool that aids in various aspects of building TrueBlocks. The program is disabled for most users, but for core developers it can be used to automatically generate help files, API interface specs, command line options parsers and many of the test case files. Generally, regular users will not use `makeClass`.

### usage

`Usage:`    makeClass [-r|-a|-o|-m|-f|-l|-t|-n|-i|-c|-v|-h] &lt;file&gt; [file...]  
`Purpose:`  Automatically writes C++ for various purposes.

`Where:`

{{<td>}}
|          | Option                           | Description                                                                   |
| -------- | -------------------------------- | ----------------------------------------------------------------------------- |
|          | files                            | one or more class definition files (required)                                 |
| &#8208;r | &#8208;&#8208;run                | run the class maker on associated <class_name(s)>                             |
| &#8208;a | &#8208;&#8208;all                | list, or run all class definitions found in the local<br/>folder              |
| &#8208;o | &#8208;&#8208;options            | export options code (check validity in the process)                           |
| &#8208;m | &#8208;&#8208;readmes            | create readme files for each tool and app                                     |
| &#8208;f | &#8208;&#8208;format             | format source code files (.cpp and .h) found in local<br/>folder and below    |
| &#8208;l | &#8208;&#8208;lint               | lint source code files (.cpp and .h) found in local<br/>folder and below      |
| &#8208;t | &#8208;&#8208;tsx                | create typescript routes                                                      |
| &#8208;n | &#8208;&#8208;nspace &lt;str&gt; | surround generated c++ code with a namespace                                  |
| &#8208;i | &#8208;&#8208;filter &lt;str&gt; | process only files whose filename or contents contain<br/>'filter'            |
| &#8208;c | &#8208;&#8208;force              | for both code generation and options generation, force<br/>writing of changes |
| &#8208;v | &#8208;&#8208;verbose            | set verbose level (optional level defaults to 1)                              |
| &#8208;h | &#8208;&#8208;help               | display this help screen                                                      |
{{</td>}}

`Notes:`

- The `--options` flag generates `COption` code for each of the various tools.
- More information on class definition files is found in the documentation.

**Source code**: [`dev_tools/makeClass`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/dev_tools/makeClass)

