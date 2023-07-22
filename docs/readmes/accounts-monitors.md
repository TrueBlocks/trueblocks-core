## chifra monitors

<!-- markdownlint-disable MD041 -->
`chifra monitors`` has two purposes: (1) to `--watch` a set of addresses. This function is in its early
stages and will be better explained elsewhere. Please see an example of what one may do with
`chifra monitors --watch`, and (2) allows one to manage existing monitored addresses.

A "monitor" is simply a file on a hard drive that represents the transactional history of a given
Ethereum address. Monitors are very small, being only the `<block_no><tx_id>` pair representing each
appearance of an address. Monitor files are only created when a user expresses interest in a
particular address. In this way, TrueBlock is able to continue to work on small desktop or even
laptop computers. (See `chifra list`.)

You may use the `--delete` command to delete (or `--undelete` if already deleted) an address. The
monitor is not removed from your computer if you delete it. It is just marked as being deleted
making it invisible to the TrueBlocks explorer.

Use the `--remove` command to permanently remove a monitor from your computer. This is an
irreversible operation and requires the monitor to have been previously deleted.

```[plaintext]
Purpose:
  Add, remove, clean, and list address monitors.

Usage:
  chifra monitors [flags] <address> [address...]

Arguments:
  addrs - one or more addresses (0x...) to process

Flags:
      --delete        delete a monitor, but do not remove it
      --undelete      undelete a previously deleted monitor
      --remove        remove a previously deleted monitor
  -C, --clean         clean (i.e. remove duplicate appearances) from monitors
  -D, --decache       removes a monitor and all associated data from the cache
  -l, --list          list monitors in the cache (--verbose for more detail)
  -w, --watch         continually scan for new blocks and extract data for monitored addresses
  -s, --sleep float   seconds to sleep between monitor passes (default 14)
  -x, --fmt string    export format, one of [none|json*|txt|csv]
  -v, --verbose       enable verbose (increase detail with --log_level)
  -h, --help          display this help screen

Notes:
  - An address must be either an ENS name or start with '0x' and be forty-two characters long.
  - If no address is presented to the --clean command, all monitors will be cleaned.
  - The --decache option will remove all cache items (blocks, txs, traces, recons) for the given address(es).
```

Data models produced by this tool:

- [monitor](/data-model/accounts/#monitor)
- [monitorclean](/data-model/admin/#monitorclean)

Links:

- [api docs](/api/#operation/accounts-monitors)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/monitors)

