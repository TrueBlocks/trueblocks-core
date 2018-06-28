### Tutorial #2

This gets a bit more expressive using the `getBlock` tool along with some other tools. First, the `ethName` tool allows you to find the address of certain well-known addresses. You can give `ethName` part of an address or a name, thus:

    ethName omisigo
    > { "symbol": "OMG", "name": "OmisiGo", "addr": "0xd26114cd6ee289accf82350c8d8487fedb8a0c07", "source": "Etherscan.io", "description": "A public Ethereum-based financial technology for use in mainstream digital wallets" }
    
or, if you add the --address or -a option you get just the address:

    ethName omisigo -a
    > 0xd26114cd6ee289accf82350c8d8487fedb8a0c07
    
Another tool we have is `getBalance` which you can feed an address and a block and it will tell you the balance of that address at that block:

    getBalance 0xd26114cd6ee289accf82350c8d8487fedb8a0c07 `ethName omisigo -a`
    > Balance for account 0xd26114cd6ee289accf82350c8d8487fedb8a0c07 at block 5871733 is 0
    > Balance for account 0xd26114cd6ee289accf82350c8d8487fedb8a0c07 at block 5871733 is 0
    
You can use `ethName` to get the address without having to remember it, and you can add your own names. Now, we will go back to `getBlock` to see what else we can do. You can use `getBlock`'s --uniq option to list the addresses in a block:

    getBlock --uniq latest
     
will return a list of addresses in the latest block (your milage may vary):

    3001001    0        0x68156ab4265672b8bcadd3ca8b646527c1436880    from
    3001001    0        0xe5f47d61b8c7a560209b5821a7eb0fda92d01cac    to
    3001001    1        0x92abcb82bfee0d9918b51d5ba2bc0b96913b1650    from
    ....
    3001001    3        0xaae65d15e74729e6072b388c2e34f67d6e327b66    log0_generator
    3001001    3        0x373c55c277b866a69dc047cad488154ab9759466    to
    3001001    3    2   0xbaf42749e027bb38ce4f23ddae8c84da8a15488f    trace2_to
    3001001    3    3   0x77a79a78c56504c6c1f7499852b6e1918a6d0ab4    trace3_to
    3001001    2        0xea674fdde714fd979de3edf0f56aa9716b898ec8    miner

