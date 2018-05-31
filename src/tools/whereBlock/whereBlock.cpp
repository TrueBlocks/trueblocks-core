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

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    //while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        SFString cachePath = blockCachePath("").Substitute("/blocks/", "/"+options.mode+"s/");
        if (isTestMode())
            cachePath = "--";
        cout << cYellow << "\nReport on " << options.mode << " locations:" << cOff << (verbose ? "" : "\n  (cache folder: " + cachePath + ")") << "\n";

        SFString list = options.getBlockNumList();
        while (!list.empty()) {
            blknum_t bn = toLongU(nextTokenClear(list, '|'));
            CFilename fileName(getBinaryFilename(bn).Substitute("/blocks/", "/"+options.mode+"s/"));
            bool exists = fileExists(fileName.getFullPath());

            SFString path = (verbose ? fileName.getFullPath() : fileName.relativePath(blockCachePath("").Substitute("/blocks/", "/"+options.mode+"s/")));
            SFString vers = getVersionFromClient();
            if (isTestMode() && verbose)
                path = "--";
            if (isTestMode())
                vers = "--";
            SFString fallback = getenv("FALLBACK");
            bool running_node = isNodeRunning();

            cout << "\t" << options.mode << " " << cTeal << padLeft(asStringU(bn),9) << cOff << " ";
            if (exists)            cout << "found at cache:  " << cTeal << path << cOff << "\n";
            else if (running_node)      cout << "found at node:   " << vers << "\n";
            else if (!fallback.empty()) cout << "found at remote: " << fallback << "\n";
            else                        cout << "was not found\n";
        }
    }
    return 0;
}
