## chifra monitors

A TrueBlock monitor is simply a file on your computer that represents the transactional history of a given Ethereum address. Monitors do not exist until you indicate your interest in a certain address. (See `chifra list`.)

You may use the `--delete` command to delete (or undelete if already deleted) an address. The monitor is not removed from your computer if you delete it. It is just marked as deleted making it invisible to the TrueBlocks explorer.

Use the `--remove` command to permanently remove a monitor from your computer. This is an irreversable operation.

```
Purpose:
  Add, remove, clean, and list address monitors.

Usage:
  chifra monitors [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to process (required)

Flags:
  -p, --appearances   export a list of appearances
  -U, --count         present only the number of records
      --clean         clean (i.e. remove duplicate appearances) from all existing monitors

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - An address must start with '0x' and be forty-two characters long.
```
**Source code**: [`apps/acctExport --appearances`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/acctExport --appearances)

