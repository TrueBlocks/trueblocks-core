### Tutorial #3

#### Stringing and Piping

QBlocks are linux command-line tools. That means you can compose the tools into each other via pipes and re-directs. In this tutorial, we will show you a little bit about that.

We'll start with `getBlock` on block 4,240,000

    getBlock 4240000
    
This returns a long string of JSON. We want to only see the list of unique addresses involved in this block

    getBlock 4240000 -u
    
This lists about 270 unique addresses for this block. Let's see if any of them owned an Omisigo token. Use `ethName` to help you remember the address of the Omisigo token contract

    ethName omisi
    
    > { "symbol": "OMG", "name": "OmisiGo", "addr": "0xd26114cd6ee289accf82350c8d8487fedb8a0c07", "source": "Etherscan.io", "description": "A public Ethereum-based financial technology for use in mainstream digital wallets" }
    
Notice that you don't have to query the entire name. Now, extract the address
    
    ethName omis -a
    
    > 0xd26114cd6ee289accf82350c8d8487fedb8a0c07
    
Now we want need a list of the addresses from the block. We use the linux `cut` command and `xargs`

    getBlock 4240000 -u | cut -f4 | xargs
    
which creates a long lists of addresses on a single line.

Now we're ready to find out if any of the addresses in the block held tokens in the Omisigo contract. We use linux command expansion to do this.

    getTokenBal `ethName omis -a` `getBlock 4240000 -u | cut -f4 | xargs`

One more step. Let's show only the non-zero token balances and let's show the values in ether (which only works if the token contract has 18 decimals)

    getTokenBal `ethName omis -a` `getBlock 4240000 -u | cut -f4 | xargs` -n --ether

There's about a billion other things you can do with the tools and applications of QBlocks. If you write code, you can do even more.

One more weird command. The ether balances of all the addresses that were involved in the latest block with no zero balances and showing in US dollars

    getBalance `getBlock -u latest | cut -f4 | xargs` -n --ether
    
In future tutorials, we will discuss how to write programming code to use the QBlocks libraries.

**Tutorials:** [1](README.md) [2](tutorial2.md) [3](tutorial3.md)
