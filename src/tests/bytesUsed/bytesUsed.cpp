/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"
#include "reporter.h"

extern bool countBytes(const SFString& fileName, void *data);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("binary");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    // while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        CReporter reporter;
        forAllFiles(blockFolder + "00/", countBytes, &reporter);
        cout << "nFiles: " << reporter.totFiles << " totalBytes:" << reporter.totBytes << "\n";
    }
    return 0;
}

//-----------------------------------------------------------------------
bool countBytes(const SFString& fileName, void *data) {

    CReporter *reporter = reinterpret_cast<CReporter*>(data);

    if (fileName.endsWith("/")) {

        if (reporter->firstSize == 0)
            reporter->firstSize = reporter->nBytes;

        reporter->totFiles += reporter->nFiles;
        reporter->nFiles = 0;

        reporter->totBytes += reporter->nBytes;
        reporter->nBytes = 0;

        forAllFiles(fileName+"*", countBytes, data);

        if (countOf('/', fileName) == 8 && reporter->firstSize > 0) {
            cout << fileName << " -"
            << " nFiles: " << reporter->nFiles
            << " nBytes: " << reporter->nBytes
            << " pct: " << ((reporter->nBytes) * 100 / reporter->firstSize)
            << "\n";
            cout.flush();

        }

    } else {
        reporter->nBytes += fileSize(fileName);
        reporter->nFiles += 1;

    }

    return true;
}
