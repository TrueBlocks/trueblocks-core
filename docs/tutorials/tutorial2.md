### Tutorial #2

#### QBlocks Tools

QBlocks consists of C++ libraries, applications, and the following 14 tools:

.    .    . [ethNames](../../src/tools/ethNames/README.md) [getBloom](../../src/tools/getBloom/README.md) [getTokenBal](../../src/tools/getTokenBal/README.md) [getBalance](../../src/tools/getBalance/README.md) [getTrace](../../src/tools/getTrace/README.md) [getLogs](../../src/tools/getLogs/README.md) [isContract](../../src/tools/isContract/README.md)  
.    .    . [whenBlock](../../src/tools/whenBlock/README.md) [ethName](../../src/tools/ethName/README.md) [getBlock](../../src/tools/getBlock/README.md) [getReceipt](../../src/tools/getReceipt/README.md) [getTrans](../../src/tools/getTrans/README.md) [whereBlock](../../src/tools/whereBlock/README.md)

#### Other Tools

Let's see what else we can do. Another tool is called `ethName`. It allows you to find the address by supplying a name or a name if you supply an address (or part of an address), thus:

    ethName omisigo

    > { "symbol": "OMG", "name": "OmisiGo", "addr": "0xd26114cd6ee289accf82350c8d8487fedb8a0c07", "source":  
    "Etherscan.io", "description": "Ethereum-based financial technology for digital wallets" }

If you add the `-a` option, you get only the address

    ethName omisigo -a

    > 0xd26114cd6ee289accf82350c8d8487fedb8a0c07
    
Another tool is called `getBalance` which, if given an address, will show the ether balance for that address at that current block:

    getBalance `ethName omisigo -a`
    
    > Balance for account 0xd26114cd6ee289accf82350c8d8487fedb8a0c07 at block 5871733 is 0
    > Balance for account 0xd26114cd6ee289accf82350c8d8487fedb8a0c07 at block 5871733 is 0
    
Notice we used `ethName` to deliver the address for the command. `ethName` makes it so you don't have to remember addresses.

Now that we know about `getBalance`, let's go back to the `getBlock` tool. `getBlock` has an option called `--addrs` which returns a list of every address involved in the given block (`latest` by default).

    getBlock --addrs 3001001
     
    > 3001001    0        0x68156ab4265672b8bcadd3ca8b646527c1436880    from
    > 3001001    0        0xe5f47d61b8c7a560209b5821a7eb0fda92d01cac    to
    > 3001001    1        0x92abcb82bfee0d9918b51d5ba2bc0b96913b1650    from
    > ....
    > 3001001    3        0xaae65d15e74729e6072b388c2e34f67d6e327b66    log0_generator
    > 3001001    3    0   0xbaf42749e027bb38ce4f23ddae8c84da8a15488f    trace0_from
    > 3001001    3    0   0xbaf42749e027bb38ce4f23ddae8c84da8a15488f    trace0_to
    > 3001001    2        0xea674fdde714fd979de3edf0f56aa9716b898ec8    miner

Notice that the address's 'purpose' in the block is also provided. This might be useful, for example, to search for occurences of your address as winning miner for the block or as a producer of an event (log). In [the next tutorial](tutorial3.md), we talk about stringing multiple of these tools together.

**Tutorials:** [1](README.md) [2](tutorial2.md) [3](tutorial3.md)
