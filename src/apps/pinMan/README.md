## chifra pins

This tool is not yet ready for production use. Please return to this page later.

### Usage

`Usage:`    chifra pins [-l|-i|-f|-a|-S|-v|-h]  
`Purpose:`  Manage pinned index of appearances and associated Bloom filters.

`Where:`

|          | Option                        | Description                                                                                 |
| -------- | ----------------------------- | ------------------------------------------------------------------------------------------- |
| &#8208;l | &#8208;&#8208;list            | list the index and Bloom filter hashes from local manifest<br/>or pinning service           |
| &#8208;i | &#8208;&#8208;init            | initialize index of appearances by downloading Bloom<br/>filters from pinning service       |
| &#8208;f | &#8208;&#8208;freshen         | freshen index of appearances using the same mode from<br/>most recent --init                |
| &#8208;a | &#8208;&#8208;all             | for --init and --freshen modes only, download full<br/>index chunks as well as Bloom filter |
| &#8208;S | &#8208;&#8208;share           | pin downloaded files to your local IPFS store, that<br/>is, share them (requires IPFS)      |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                                           |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                                            |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                                                    |

`Notes:`

- One of `--list`, `--init`, or `--freshen` is required.
- the `--share` option only works if the IPFS executable is in your path.

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
