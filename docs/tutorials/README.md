### Basics - Tutorial #1

If you haven't already done so, please [intall and build the QBlocks code](../../src/other/install/INSTALL.md).

QBlocks is a collection of C++ libraries, tools, and applications. In this document, we present a few tutorials to get you acquainted with how QBlocks works. Of course, you will build your own tools use QBlocks, so it works however you want it to work. See our CONTRIBUTORS documentation to get started.

#### Using getBlock

The first tool we'll discuss is called `getBlock` which returns Ethereum block data given a block number, a range of block numbers, or a block hash. For example, the command

    getBlock 3001001

returns the JSON data representing block 3,001,001. While the command

    getBlock 3001001-3001101

returns 100 blocks worth of JSON data for the block range 3,001,001 and 3,001,101 non-inclusive, and this command

    getBlock 0xb4fbadf8ea452b139718e2700dc1135cfc81145031c84b7ab27cd710394f7b38

return JSON data for block 999,999 which happens to have the above hash.

As with all QBlocks tools, you can get a full listing of the command options by entering the `--help` option

    getBlock --help
    
shows the help file. Looking at some of the options, let's look at the `--hash_only` option which returns only the transactions hashes in the block as opposed to the full data for each transaction.

    getBlock 1001001 --hash_only
    
The following command shows all addresses involved in the block along with the 'purpose' that address played in the block. You may use the `--uniq` option to limit the display to a unique list of addresses (as opposed to all addresses).

    getBlock 1001001 --addrs

The 'purpose' of the address means where in the block the address appears. We think there are 14 different possible purposes for an address in a block:

    [ miner | from | to | creation | input | log_generator | log_topics | log_data | trace_from |
      trace_to | trace_refundAddr | trace_creation | trace_self-destruct | trace_input ]

[In the next tutorial](tutorial2.md) we'll start getting a bit more complicated.

**Tutorials:** [1](README.md) [2](tutorial2.md) [3](tutorial3.md)
