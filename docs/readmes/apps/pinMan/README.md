## chifra pins

This tool is not yet ready for production use. Please return to this page later.

### usage

`Usage:`    chifra pins [-l|-i|-n|-p|-v|-h]  
`Purpose:`  Manage pinned index of appearances and associated Bloom filters.

`Where:`

{{<td>}}
|          | Option                        | Description                                                                       |
| -------- | ----------------------------- | --------------------------------------------------------------------------------- |
| &#8208;l | &#8208;&#8208;list            | list the index and Bloom filter hashes from local manifest<br/>or pinning service |
| &#8208;i | &#8208;&#8208;init            | initialize local index by downloading Bloom filters<br/>from pinning service      |
| &#8208;n | &#8208;&#8208;init_all        | initialize local index by downloading both Bloom filters<br/>and index chunks     |
| &#8208;p | &#8208;&#8208;pin_locally     | pin all local files in the index to an IPFS store (requires<br/>IPFS)             |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                                 |
| &#8208;v | &#8208;&#8208;verbose         | set verbose level (optional level defaults to 1)                                  |
| &#8208;h | &#8208;&#8208;help            | display this help screen                                                          |
{{</td>}}

`Notes:`

- One of `--list`, `--init`, or `--init_all` is required.
- the `--pin_locally` option only works if the IPFS executable is in your path.

**Source code**: [`apps/pinMan`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/pinMan)

