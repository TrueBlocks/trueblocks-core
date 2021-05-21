## chifra pins

This tool is not yet ready for production use. Please return to this page later.

### usage

`Usage:`    chifra pins [-l|-i|-c|-f|-v|-h]  
`Purpose:`  Manage pinned index of appearances and associated bloom filters.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
| -l | --list | list the index and bloom filter hashes from local manifest or pinning service |
| -i | --init | initialize the local index by downloading bloom filters from the pinning service |
| -c | --compare | report differences (if any) between the manifest and pinning service |
| -f | --freshen | freshen the manifest from the hash found at the smart contract |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- One of --list, --init, --compare, or --freshen is required.

**Source code**: [`apps/pinMan`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/pinMan)

