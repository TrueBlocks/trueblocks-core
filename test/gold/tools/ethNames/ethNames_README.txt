ethNames argc: 2 [1:--readme] 
ethNames --readme 
### Usage

`Usage:`    chifra names [-e|-m|-l|-c|-p|-n|-a|-s|-g|-v|-h] &lt;term&gt; [term...]  
`Purpose:`  Query addresses or names of well known accounts.

`Where:`

{{<td>}}
|          | Option                             | Description                                                                               |
| -------- | ---------------------------------- | ----------------------------------------------------------------------------------------- |
|          | terms                              | a space separated list of one or more search terms<br/>(required)                         |
| &#8208;e | &#8208;&#8208;expand               | expand search to include all fields (default searches<br/>name, address, and symbol only) |
| &#8208;m | &#8208;&#8208;match_case           | do case-sensitive search                                                                  |
| &#8208;l | &#8208;&#8208;all                  | include all accounts in the search                                                        |
| &#8208;c | &#8208;&#8208;custom               | include your custom named accounts                                                        |
| &#8208;p | &#8208;&#8208;prefund              | include prefund accounts                                                                  |
| &#8208;n | &#8208;&#8208;named                | include well know token and airdrop addresses in the<br/>search                           |
| &#8208;a | &#8208;&#8208;addr                 | display only addresses in the results (useful for scripting)                              |
| &#8208;s | &#8208;&#8208;collections          | display collections data                                                                  |
| &#8208;g | &#8208;&#8208;tags                 | export the list of tags and subtags only                                                  |
| ###      | Hidden options                     |                                                                                           |
| &#8208;u | &#8208;&#8208;to_custom            | for editCmd only, is the edited name a custom name<br/>or not                             |
| &#8208;C | &#8208;&#8208;clean                | clean the data (addrs to lower case, sort by addr)                                        |
| &#8208;A | &#8208;&#8208;autoname &lt;str&gt; | an address assumed to be a token, added automatically<br/>to names database if true       |
| ###      | Hidden options                     |                                                                                           |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;      | export format, one of [none, json, txt, csv, api]                                         |
| &#8208;v | &#8208;&#8208;verbose              | set verbose level (optional level defaults to 1)                                          |
| &#8208;h | &#8208;&#8208;help                 | display this help screen                                                                  |
{{</td>}}

`Notes:`

- The tool will accept up to three terms, each of which must match against any field in the database.
- The `--match_case` option enables case sensitive matching.

