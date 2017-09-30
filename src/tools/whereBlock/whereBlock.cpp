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

        if (!options.alone)
            cout << cYellow << "\nReport on block locations:" << cOff << (verbose ? "" : "\n  (cache folder: " + blockCachePath("") + ")") << "\n";
        for (uint32_t i = 0 ; i < options.blocks.getCount() ; i++ ) {

            blknum_t block = options.blocks[i];
            CFilename fileName(getBinaryFilename1(block));
            bool exists = fileExists(fileName.getFullPath());

            if (options.alone) {
                // When running in 'alone' mode, only report items in the cache
                if (exists)
                    cout << fileName.getFullPath() << "\n";

            } else {

                SFString path = (verbose ? fileName.getFullPath() : fileName.relativePath(getStorageRoot()));
                SFString fallback = getenv("FALLBACK");
                bool running_node = isNodeRunning();

                cout << "\tblock " << cTeal << padLeft(asStringU(block),9) << cOff << " ";
                     if (exists)            cout << "found at cache:  " << cTeal << path << cOff << "\n";
                else if (running_node)      cout << "found at node:   " << getVersionFromClient() << "\n";
                else if (!fallback.empty()) cout << "found at remote: " << fallback << "\n";
                else                        cout << "was not found\n";
            }
        }
    }
    return 0;
}
