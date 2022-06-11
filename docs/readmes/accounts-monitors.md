## chifra monitors

A TrueBlocks monitor tool has two purposes. The first is to `--watch` a set of addresses. This function is in its early stages and will be better explained elsewhere. Please see an example of what one may do with `chifra monitors --watch` [here](https://tokenomics.io/).

A "monitor" is simply a file on a hard drive that represents the transactional history of a given Ethereum address. Monitors are very small, being only the `<block_no><tx_id>` pair representing each appearance of an address. Monitor files are only created when a user expresses interest in a particular address. In this way, TrueBlock is able to continue to work on small desktop or even laptop computers. (See `chifra list`.)

You may use the `--delete` command to delete (or `--undelete` if already deleted) an address. The monitor is not removed from your computer if you delete it. It is just marked as being deleted making it invisible to the TrueBlocks explorer.

Use the `--remove` command to permanently remove a monitor from your computer. This is an irreversible operation and requires the monitor to have been previously deleted.

```[plaintext]
Purpose:
  Add, remove, clean, and list address monitors.

Usage:
  chifra monitors [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to process

Flags:
      --clean        clean (i.e. remove duplicate appearances) from monitors
      --delete       delete a monitor, but do not remove it
      --undelete     undelete a previously deleted monitor
      --remove       remove a previously deleted monitor
      --watch        continually scan for new blocks and extract data for monitored addresses
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -v, --verbose      enable verbose (increase detail with --log_level)
  -h, --help         display this help screen

Notes:
  - An address must start with '0x' and be forty-two characters long.
  - If no address is presented to the --clean command, all monitors will be cleaned.
```

**Source code**: [`internal/monitors`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/monitors)

