/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
bool COptions::handleImport(void) const {

    ASSERT(fileExists("./import.txt"));
    ASSERT(filenames.size() == 1);

    CAppearanceArray_base dataArray;
    if (!handleRead("Reading", fileExists(monitors[0].name), dataArray))
        return false;

    CStringArray lines;
    string_q contents;
    asciiFileToString("./import.txt", contents);
    size_t nRecords = explode(lines, contents, '\n');
    for (auto line : lines) {
        CAppearance_base item(line);
        if (item.blk > 0) {
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

    if (!handleWrite(monitors[0].name, dataArray, NULL))
        return false;

    return true;
}

