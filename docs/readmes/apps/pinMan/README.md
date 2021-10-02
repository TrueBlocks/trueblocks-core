## chifra pins

This tool is not yet ready for production use. Please return to this page later.

### usage

`Usage:`    chifra init [-l|-i|-f|-a|-S|-v|-h]  
`Purpose:`  Manage pinned index of appearances and associated Bloom filters.

`Where:`

{{<td>}}
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
{{</td>}}

`Notes:`

- One of `--list`, `--init`, or `--freshen` is required.
- the `--share` option only works if the IPFS executable is in your path.

**Source code**: [`apps/pinMan`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/pinMan)

