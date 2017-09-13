/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

/*-------------------------------------------------------------------------
 * Name:    Simple
 * Purpose: To provide the easiest introduction to the QuickBlocks library.
 *          Simply request a block from Infura and print it to the screen.
 *------------------------------------------------------------------------*/
int main(int argc, const char *argv[]) {

    // Initialize the system and tell it where to find the blockchain data.
    etherlib_init("infura");

    // Request the 3,500,000th block from Infura. Store it in 'block'
    CBlock block;
    getBlock(block, 3500000);

    // Print the block to the screen
    cout << block << "\n";

    return 0;
}
