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

        for (uint32_t i = 0 ; i < options.blocks.getCount() ; i++ ) {
            CFilename fileName(getBinaryFilename1(options.blocks[i]));
            bool exists = fileExists(fileName.getFullPath());
            if (exists && options.alone) {
                cout << fileName.getFullPath() << "\n";
            } else if (exists) {
                cout << "File " << fileName.relativePath(getStorageRoot()) << " found in cache.\n";
            } else {
                cout << "The block " << options.blocks[i] << " was not found in the cache.\n";
            }
        }
    }
    return 0;
}
