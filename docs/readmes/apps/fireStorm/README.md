## chifra explore

`chifra explore` opens Etherscan (and other explorers -- including our own) to the block, transaction hash, or address you specify. It's a handy (configurable) way to open an explorer from the command line, nothing more.

### usage

`Usage:`    chifra explore [-l|-g|-v|-h] &lt;term&gt; [term...]
`Purpose:`  Open an explorer for a given address, block, or transaction.

`Where:`

| | Option | Description |
| :----- | :----- | :---------- |
|  | terms | one or more addresses, names, block, or transaction identifiers |
| -l | --local | open the local TrueBlocks explorer |
| -g | --google | search google excluding popular blockchain explorers |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

**Source code**: [`apps/fireStorm`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/fireStorm)

