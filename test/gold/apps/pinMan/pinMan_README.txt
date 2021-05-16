pinMan argc: 2 [1:-th] 
pinMan -th 
### Usage

`Usage:`    chifra pins [-c|-i|-f|-s|-v|-h]  
`Purpose:`  Manage pinned index of appearances and associated bloom filters.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
| -c | --compare | report differences (if any) between the manifest and pinning service |
| -i | --init | initialize the local index by downloading bloom filters from the pinning service |
| -f | --freshen | freshen the manifest from the hash found at the smart contract |
| -s | --sleep &lt;double&gt; | the number of seconds to sleep between requests during init (default .25) |
| -x | --fmt &lt;val&gt; | export format, one of *[ none \| json\* \| txt \| csv \| api ]* |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

