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

    string_q contents = asciiFileToString("./import.txt");
    while (!contents.empty() && !shouldQuit()) {
        string_q line  = nextTokenClear(contents,'\n');
        CAcctCacheItem item(line);
        if (item.blockNum > 0) {
            dataArray.push_back(item);
            cerr << "\tImporting item " << item << "\n";
        }
    }
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

