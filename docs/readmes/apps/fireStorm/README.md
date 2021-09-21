## chifra explore

`chifra explore` opens Etherscan (and other explorers -- including our own) to the block, transaction hash, or address you specify. It's a handy (configurable) way to open an explorer from the command line, nothing more.

### usage

`Usage:`    chifra explore [-l|-g|-v|-h] &lt;term&gt; [term...]  
`Purpose:`  Open an explorer for a given address, block, or transaction.

`Where:`

{{<td>}}
|          | Option                        | Description                                                         |
| -------- | ----------------------------- | ------------------------------------------------------------------- |
|          | terms                         | one or more addresses, names, block, or transaction<br/>identifiers |
| &#8208;l | &#8208;&#8208;local           | open the local TrueBlocks explorer                                  |
| &#8208;g | &#8208;&#8208;google          | search google excluding popular blockchain explorers                |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                   |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                    |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                            |
{{</td>}}

**Source code**: [`apps/fireStorm`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/fireStorm)

