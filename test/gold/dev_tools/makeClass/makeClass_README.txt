makeClass argc: 2 [1:--readme] 
makeClass --readme 
### Usage

`Usage:`    makeClass [-a|-o|-m|-f|-l|-t|-v|-h] &lt;file&gt; [file...]  
`Purpose:`  Automatically writes C++ for various purposes.

`Where:`

{{<td>}}
|          | Option                        | Description                                                                |
| -------- | ----------------------------- | -------------------------------------------------------------------------- |
|          | files                         | one or more class definition files (required)                              |
| &#8208;a | &#8208;&#8208;all             | list, or run all class definitions found in the local<br/>folder           |
| &#8208;o | &#8208;&#8208;options         | export options code (check validity in the process)                        |
| &#8208;m | &#8208;&#8208;readmes         | create readme files for each tool and app                                  |
| &#8208;f | &#8208;&#8208;format          | format source code files (.cpp and .h) found in local<br/>folder and below |
| &#8208;l | &#8208;&#8208;lint            | lint source code files (.cpp and .h) found in local<br/>folder and below   |
| &#8208;t | &#8208;&#8208;tsx             | create typescript routes                                                   |
| ###      | Hidden options                |                                                                            |
| &#8208;d | &#8208;&#8208;dump            | dump any classDefinition config tomls to screen and<br/>quit               |
| &#8208;A | &#8208;&#8208;openapi         | export openapi.yaml file for API documentation                             |
| ###      | Hidden options                |                                                                            |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                          |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                           |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                                   |
{{</td>}}

`Notes:`

- The `--options` flag generates `COption` code for each of the various tools.
- More information on class definition files is found in the documentation.

