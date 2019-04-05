/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//---------------------------------------------------------------
bool visitIndexFiles(const string_q& path, void *data) {

    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitIndexFiles, data);

    } else {

        cerr << "Searching file : " << path << "\r"; cerr.flush();

        // Filenames take the form 'start-end.[txt|bin]' where both 'start' and 'end'
        // are inclusive. Silently skips unknown files in the folder (such as shell scripts).
        if (!contains(path, "-") || (!endsWith(path, ".txt") && !endsWith(path, ".bin")))
            return !shouldQuit();

        // Pick up some useful data for either method...
        COptions *options = reinterpret_cast<COptions*>(data);
        options->firstBlockInFile = bnFromPath(path, options->lastBlockInFile);
        ASSERT(options->firstBlockInFile != NOPOS);
        ASSERT(options->lastBlockInFile != NOPOS);

        if (options->startScrape > options->lastBlockInFile)
            return options->processTooEarlyFile(path);

        if (options->new_version)
            return options->visitBinaryFile(path, data);
        else
            return options->visitAsciiFile(path, data);

    }
    ASSERT(0); // should not happen
    return !shouldQuit();
}

//---------------------------------------------------------------
bool COptions::processTooEarlyFile(const string_q& path) {

    uint64_t nRecords = NOPOS;
    if (new_version) {
    } else {
        nRecords = fileSize(path) / sizeof(CIndexRecord);
    }

    addrStats.nSeen += nRecords;
    if (startScrape > lastBlockInFile) {
        addrStats.nSkipped += nRecords;
        if (addrStats.nHit == 0) {
            for (auto monitor : monitors) {
                string_q fn = getMonitorPath(monitor.address);
                if (fileExists(fn))
                    addrStats.nHit += fileSize(fn) / (sizeof(uint64_t) * 2);
            }
        }
    }

    return !shouldQuit();
}
