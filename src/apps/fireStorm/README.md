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

All tools accept the following additional flags, although in some cases, they have no meaning.

| Command     | Description                                                   |
| ----------- | ------------------------------------------------------------- |
| --version   | display the current version of the tool                       |
| --wei       | export values in wei (the default)                            |
| --ether     | export values in ether                                        |
| --dollars   | export values in US dollars                                   |
| --raw       | pass raw RPC data directly from the node with no processing   |
| --to_file   | write the results to a temporary file and return the filename |
| --output fn | write the results to file 'fn' and return the filename        |
| --file fn   | specify multiple sets of command line options in a file       |

<small>*For the `--file fn` option, you may place a series of valid command lines in a file using any of the above flags. In some cases, this may significantly improve performance. A semi-colon at the start of any line makes that line a comment.*</small>
