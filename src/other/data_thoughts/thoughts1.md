### Types of included transactions

1. I think there might be 'types' we can associate to transactions based on where we find an address in a transaction. Types are relative to the address we're querying. The types are the same as those found in getBlock --addrs

2. We need an index of addresses per block. It's astonishing how slow it is to scan 6,500,000 blocks. It simply does not work

3.  Creation of QuickBlocks bloom filters can easily start at any block. User can later decide to backfill

4. 

