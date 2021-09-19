cmdLineTest argc: 2 [1:-th] 
cmdLineTest -th 
### Usage

`Usage:`     [-b|-i|-u|-s|-r|-l|-v|-h] testNum optionalMode  
`Purpose:`  Tests various command line behavior.
        
`Where:`

{{<td>}}
|          | Option                            | Description                                                         |
| -------- | --------------------------------- | ------------------------------------------------------------------- |
|          | testNum                           | the number of the test to run (required)                            |
|          | optionalMode                      | an optional mode with ~! start                                      |
| &#8208;b | &#8208;&#8208;bool &lt;bool&gt;   | enter a boolean value (either '0', '1', 'false', or<br/>'true')     |
| &#8208;i | &#8208;&#8208;int &lt;int&gt;     | enter any numeric value                                             |
| &#8208;u | &#8208;&#8208;uint &lt;uint&gt;   | enter any numeric value greater than or equal to zero               |
| &#8208;s | &#8208;&#8208;string &lt;str&gt;  | enter any value                                                     |
| &#8208;r | &#8208;&#8208;range &lt;range&gt; | enter a range of numeric values                                     |
| &#8208;l | &#8208;&#8208;list &lt;list&gt;   | enter a list of value separated by commas (no spaces<br/>or quoted) |
| ###      | Hidden options                    |                                                                     |
| &#8208;d | &#8208;&#8208;hiddenOption        | a hidden option with an alternative hot key                         |
| ###      | Hidden options                    |                                                                     |
| &#8208;v | &#8208;&#8208;verbose             | set verbose level (optional level defaults to 1)                    |
| &#8208;h | &#8208;&#8208;help                | display this help screen                                            |
{{</td>}}

