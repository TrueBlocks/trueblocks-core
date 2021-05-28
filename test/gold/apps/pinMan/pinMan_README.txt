pinMan argc: 2 [1:-th] 
pinMan -th 
### Usage

`Usage:`    chifra pins [-l|-i|-c|-k|-p|-v|-h]  
`Purpose:`  Manage pinned index of appearances and associated bloom filters.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
| -l | --list | list the index and bloom filter hashes from local manifest or pinning service |
| -i | --init | initialize the local index by downloading bloom filters from the pinning service |
| -c | --compare | report differences (if any) between the manifest and pinning service |
| -k | --chunks | for --init option only, download both blooms (the default) and index chunks |
| -p | --repin | for --init option only, repin the bloom (or chunk) after downloading |
|####|Hidden options||
| -s | --sleep &lt;double&gt; | the number of seconds to sleep between requests during init (default .25) |
| -r | --remote | applicable only to --list mode, recover hash list from pinning service (local otherwise) |
|####|Hidden options||
| -x | --fmt &lt;val&gt; | export format, one of *[ none \| json\* \| txt \| csv \| api ]* |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- One of --list, --init, --compare, or --freshen is required.
- the `repin` option only works if the ipfs executable is in your path.

