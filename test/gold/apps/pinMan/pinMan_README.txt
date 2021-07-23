pinMan argc: 2 [1:-th] 
pinMan -th 
### Usage

`Usage:`    chifra pins [-l|-i|-k|-p|-v|-h]  
`Purpose:`  Manage pinned index of appearances and associated Bloom filters.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
| -l | --list | list the index and Bloom filter hashes from local manifest or pinning service |
| -i | --init | initialize local index by downloading Bloom filters from pinning service |
| -k | --init_all | initialize local index by downloading both Bloom filters and index chunks |
| -p | --pin_locally | pin all local files in the index to an IPFS store (requires IPFS) |
|####|Hidden options||
| -s | --sleep &lt;double&gt; | the number of seconds to sleep between requests during init (default .25) |
| -r | --remote | applicable only to --list mode, recover the manifest from pinning service |
|####|Hidden options||
| -x | --fmt &lt;val&gt; | export format, one of *[ none \| json\* \| txt \| csv \| api ]* |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- One of `--list`, `--init`, or `--init_all` is required.
- the `--pin_locally` option only works if the IPFS executable is in your path.

