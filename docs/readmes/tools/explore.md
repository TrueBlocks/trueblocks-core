## chifra explore

`chifra list` takes one or more addresses, queries the index of appearances, and builds a TrueBlocks 'monitor'. A monitor is a file that represents your interest in those particular addresses. The first time you create a monitor takes a few minutes, but the information is cached, so subsequent queries are much faster.

Note that `chifra list` does not extract transactional data from the chain. This is accomplished with `chifra export`. In fact, `chifra list` is just a shortcut of the command `chifra export --appearances` and may be used interchangably.

### usage

`Usage:`    chifra list &lt;address&gt; [address...]  
`Purpose:`  List appearances for the given address(es).

`Where:`  

| Hotkey | Option | Description |
| -------: | :------- | :------- |
|  | addrs | one or more addresses (0x...) to export (required) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

`Notes:`

- `addresses` must start with '0x' and be forty two characters long.

**Source code**: [`apps/acctExport`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/acctExport)
