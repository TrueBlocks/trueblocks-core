/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

extern void loadWatches(const CToml& toml, CAccountWatchArray& array);
//-------------------------------------------------------------------------
bool processSplit(COptions& options) {

    if (!fileExists("./config.toml"))
        cerr << "Config file ./config.toml not found. Quitting...\n";

    CToml toml("./config.toml");

    CAccountWatchArray array;
    loadWatches(toml, array);

    for (uint32_t i=0;i<array.getCount();i++) {
        cout << array[i].Format("[{INDEX}]\t[{NAME}]\t[{ADDRESS}]\n");
    }

    return true;
}

//-----------------------------------------------------------------------
void loadWatches(const CToml& toml, CAccountWatchArray& array) {

    SFString watchStr = toml.getConfigStr("watches", "list", "");
    if (watchStr.empty()) {
        cout << "Empty list of watches. Quitting.\r\n";
        exit(0);
    }

    uint32_t cnt = 0;
    char *p = cleanUpJson((char *)watchStr.c_str());
    while (p && *p) {
        CAccountWatch watch;
        uint32_t nFields = 0;
        p = watch.parseJson(p, nFields);
        if (nFields) {
            // cleanup and report on errors
            SFString msg;
            watch.index = cnt++;
            watch.address = fixAddress(toLower(watch.address));
            if (!isAddress(watch.address)) {
                cerr << "invalid address " << watch.address << "\n";
                exit(0);
            }
            array[array.getCount()] = watch;
        }
    }
    return;
}
