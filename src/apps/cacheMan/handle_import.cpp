/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
bool COptions::handleImport(void) const {

    ASSERT(fileExists("./import.txt"));

    CAcctCacheItemArray dataArray;
    if (!handleRead("Reading", fileExists(filenames[0]), dataArray))
        return false;

    CStringArray lines;
    string_q contents = asciiFileToString("./import.txt");
    size_t nRecords = explode(lines, contents, '\n');
    for (auto line : lines) {
        CAcctCacheItem item(line);
        if (item.blockNum > 0) {
            dataArray.push_back(item);
            if (!(dataArray.size() % 13)) {
                cerr << "\tImporting record " << dataArray.size() << " of " << nRecords << "\r";
                cerr.flush();
            }
        }
    }
    cerr << "\n";
    if (!isTestMode()) {
        copyFile("./import.txt", "import.bak");
        remove("./import.txt");
    }

    cerr << "\tSorting...";
    sort(dataArray.begin(), dataArray.end());
    cerr << "done\n";

    if (!handleWrite(filenames[0], dataArray, NULL))
        return false;

    return true;
}

