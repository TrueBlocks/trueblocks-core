### #1 getBlock Basics

This tutorial assumes you've already built QuickBlocks and added the QuickBlocks folder to your path. Build instructions are here. The command

    getBlock 1001001

will return JSON data representing block 1,001,001. The command:

    getBlock 1001001-1001101

will return the 100 blocks between blocks 1,001,001 and 1,001,101 non-inclusive, but getBlocks has a lot of options:

    getBlock --help
    
shows the help file. This command

    getBlock 1001001 --hash_only
    
unlike the default, shows only the transaction hashes for the given block, while this command shows every address involved in the block along with the 'purpose' of the address in that block.

    getBlock 1001001 --addresses

An addresses's purpose in a block can be one of

    [ miner | to | from | input | ... ]
