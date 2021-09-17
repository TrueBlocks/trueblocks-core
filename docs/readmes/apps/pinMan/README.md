## chifra pins

This tool is not yet ready for production use. Please return to this page later.

### usage

`Usage:`    chifra pins [-l|-i|-n|-p|-v|-h]
`Purpose:`  Manage pinned index of appearances and associated Bloom filters.

`Where:`

{{<td>}}
|     | Option            | Description                                                                       |
| --- | ----------------- | --------------------------------------------------------------------------------- |
| -l  | --list            | list the index and Bloom filter hashes from local manifest<br/>or pinning service |
| -i  | --init            | initialize local index by downloading Bloom filters<br/>from pinning ervice       |
| -n  | --init_all        | initialize local index by downloading both Bloom filters<br/>and index chunks     |
| -p  | --pin_locally     | pin all local files in the index to an IPFS store (requires<br/>IPFS)             |
| -x  | --fmt &lt;val&gt; | export format, one of [none, json, txt, csv, api]                                 |
| -v  | --verbose         | set verbose level (optional level defaults to 1)                                  |
| -h  | --help            | display this help screen                                                          |
{{</td>}}

`Notes:`

- One of `--list`, `--init`, or `--init_all` is required.
- the `--pin_locally` option only works if the IPFS executable is in your path.

**Source code**: [`apps/pinMan`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/pinMan)

