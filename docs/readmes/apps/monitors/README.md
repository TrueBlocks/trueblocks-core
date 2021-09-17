## chifra monitors

A TrueBlock monitor is simply a file on your computer that represents the transactional history of a given Ethereum address. Monitors do not exist until you indicate your interest in a certain address. (See `chifra list`.)

You may use the `--delete` command to delete (or undelete if already deleted) an address. The monitor is not removed from your computer if you delete it. It is just marked as deleted making it invisible to the TrueBlocks explorer.

Use the `--remove` command to permanently remove a monitor from your computer. This is an irreversable operation.

### usage

`Usage:`    chifra monitors [-d|-r|-v|-h] &lt;address&gt; [address...]  
`Purpose:`  Delete, undelete, and remove previously created monitors.

`Where:`

|     | Option    | Description                                                          |
| --- | --------- | -------------------------------------------------------------------- |
|     | addrs     | one or more addresses (0x...) to export (required)                   |
|     | --delete  | delete a previously created monitor (or undelete if already deleted) |
|     | --remove  | remove a previously deleted monitor                                  |
| -v  | --verbose | set verbose level (optional level defaults to 1)                     |
| -h  | --help    | display this help screen                                             |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.

**Source code**: [`apps/acctExport`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/acctExport)
