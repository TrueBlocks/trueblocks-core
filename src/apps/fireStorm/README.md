## chifra explore

`chifra explore` opens Etherscan (and other explorers -- including our own) to the block, transaction hash, or address you specify. It's a handy (configurable) way to open an explorer from the command line, nothing more.

### Usage

`Usage:`    chifra explore [-l|-g|-h] &lt;term&gt; [term...]  
`Purpose:`  Open an explorer for one or more addresses, blocks, or transactions.

`Where:`

|          | Option               | Description                                                         |
| -------- | -------------------- | ------------------------------------------------------------------- |
|          | terms                | one or more addresses, names, block, or transaction<br/>identifiers |
| &#8208;l | &#8208;&#8208;local  | open the local TrueBlocks explorer                                  |
| &#8208;g | &#8208;&#8208;google | search google excluding popular blockchain explorers                |
| &#8208;h | &#8208;&#8208;help   | display this help screen                                            |

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

| Command     | Description                                                   |
| ----------- | ------------------------------------------------------------- |
| --version   | display the current version of the tool                       |
| --nocolor   | turn off colored display                                      |
| --wei       | specify value in wei (the default)                            |
| --ether     | specify value in ether                                        |
| --dollars   | specify value in US dollars                                   |
| --raw       | report JSON data from the node with minimal processing        |
| --to_file   | write the results to a temporary file and return the filename |
| --output:fn | write the results to file 'fn' and return the filename        |
| --file:fn   | specify multiple sets of command line options in a file.      |

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source code**: [`apps/fireStorm`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/fireStorm)

