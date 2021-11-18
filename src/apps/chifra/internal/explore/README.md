## chifra explore

`chifra explore` opens Etherscan (and other explorers -- including our own) to the block, transaction hash, or address you specify. It's a handy (configurable) way to open an explorer from the command line, nothing more.

```[plaintext]
Purpose:
  Open a local or remote explorer for one or more addresses, blocks, or transactions.

Usage:
  chifra explore [flags] <term> [term...]

Arguments:
  terms - one or more address, name, block, or transaction identifier

Flags:
  -l, --local        open the local TrueBlocks explorer
  -g, --google       search google excluding popular blockchain explorers
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -v, --verbose      enable verbose (increase detail with --log_level)
  -h, --help         display this help screen
```

**Source code**: [`explore.go`](https://github.com/TrueBlocks/trueblocks-core/tree/develop/src/apps/chifra/internal/explore.go)

