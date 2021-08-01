## chifra pins

This tool is not yet ready for production use. Please return to this page later.

### usage

`Usage:`    chifra pins [-l|-i|-k|-p|-v|-h]
`Purpose:`  Manage pinned index of appearances and associated Bloom filters.

`Where:`

| | Option | Description |
| :----- | :----- | :---------- |
| -l | --list | list the index and Bloom filter hashes from local manifest or pinning service |
| -i | --init | initialize local index by downloading Bloom filters from pinning service |
| -k | --init_all | initialize local index by downloading both Bloom filters and index chunks |
| -p | --pin_locally | pin all local files in the index to an IPFS store (requires IPFS) |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

`Notes:`

- One of `--list`, `--init`, or `--init_all` is required.
- the `--pin_locally` option only works if the IPFS executable is in your path.

**Source code**: [`apps/pinMan`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/pinMan)

