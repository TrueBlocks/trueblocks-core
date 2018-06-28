### Tutorial #2

This gets a bit more expressive using the `getBlock` tool along with some other tools. First, the `ethName` tool allows you to find the address of certain well-known addresses. You can give `ethName` part of an address or a name, thus:

    ethName omisigo

which returns

    { "symbol": "OMG", "name": "OmisiGo", "addr": "0xd26114cd6ee289accf82350c8d8487fedb8a0c07", "source": "Etherscan.io", "description": "A public Ethereum-based financial technology for use in mainstream digital wallets" }
    
or, if you add the --address or -a option you get just the address:

    0xd26114cd6ee289accf82350c8d8487fedb8a0c07
    
Another tool we have is `getBalance` which you can feed an address and a block and it will tell you the balance of that address at that block:

    getBalance 0xd26114cd6ee289accf82350c8d8487fedb8a0c07 `ethName omisigo -a`
    
You can use `ethName` to get the address without having to remember. Now, we will go back to `getBlock`. You can use `getBlock`'s --uniq option to list the addresses in a block:

    getBlock --uniq latest
     
will return a list of addresses in the latest block (your milage may vary):

    3001001	0		0x68156ab4265672b8bcadd3ca8b646527c1436880	from
    3001001	0		0xe5f47d61b8c7a560209b5821a7eb0fda92d01cac	to
    3001001	1		0x92abcb82bfee0d9918b51d5ba2bc0b96913b1650	from
    ....
    3001001	2		0xca3de95c2b3fcb03f0559c338bfea9cf843fcbbb	from

