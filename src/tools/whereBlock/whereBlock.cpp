/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("binary");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    //while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        SFString fileName = getBinaryFilename1(options.block);
        bool exists = fileExists(fileName);
        if (exists && options.alone) {
            cout << fileName << "\n";
        } else if (exists) {
            cout << "File " << fileName.Substitute(BLOCK_CACHE,"./") << " found in cache.\n";
        } else {
            cout << "The block " << options.block << " was not found in the cache.\n";
        }
    }
    return 0;
}
