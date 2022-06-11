## chifra monitors

A TrueBlocks monitor tool has two purposes. The first is to `--watch` a set of addresses. This function is in its early stages and will be better explained elsewhere. Please see an example of what one may do with `chifra monitors --watch` [here](https://tokenomics.io/).

A "monitor" is simply a file on a hard drive that represents the transactional history of a given Ethereum address. Monitors are very small, being only the `<block_no><tx_id>` pair representing each appearance of an address. Monitor files are only created when a user expresses interest in a particular address. In this way, TrueBlock is able to continue to work on small desktop or even laptop computers. (See `chifra list`.)

You may use the `--delete` command to delete (or `--undelete` if already deleted) an address. The monitor is not removed from your computer if you delete it. It is just marked as being deleted making it invisible to the TrueBlocks explorer.

Use the `--remove` command to permanently remove a monitor from your computer. This is an irreversible operation and requires the monitor to have been previously deleted.

[{USAGE}]
[{FOOTER}]
