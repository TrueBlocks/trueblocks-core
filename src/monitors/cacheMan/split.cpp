/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

extern void loadWatches(const CToml& toml, CAccountWatchArray& array);
extern void oneWatch(const CAccountWatch& watch);
//-------------------------------------------------------------------------
bool processSplit(COptions& options) {

    if (!fileExists("./config.toml")) {
        cerr << "Config file ./config.toml not found. Quitting...\n";
        return false;
    }

    CToml toml("./config.toml");
    CAccountWatchArray array;
    loadWatches(toml, array);

    cout << iMagenta << "\n" << array.getCount() << " watched accounts were found in the file: "
            << bTeal << options.filenames[0] << "\n" << cOff;
    for (uint32_t i=0;i<=array.getCount()/2;i++) {
        oneWatch(array[i]);
        if ((i+array.getCount()/2)+1<array.getCount())
            oneWatch(array[(i+array.getCount()/2)+1]);
        cout << "\n";
    }

    cerr << iMagenta << "The results will be placed in the folder ./split.\n" << cOff;
    cerr << bTeal << "Continue? (y=yes) > " << cOff;
    int ch = (isTestMode() ? 'y' : getchar());
    if (ch != 'y' && ch != 'Y') {
        cerr << "Split request ignored.\n";
        return false;  // return false so we don't continue
    }

    SFString source = options.filenames[0].Substitute("cache/", "split/").Substitute(".acct.bin", ".junk");
    cout << "\nSplitting the file " << options.filenames[0] << " into the folder ./split.\n";
    if (!establishFolder("split/"))
        cout << "Could not create folder.\n";
    else
        copyFile(options.filenames[0], source);

    CAcctCacheItemArray theData;
    SFArchive cache(READING_ARCHIVE);
    if (!cache.Lock(source, binaryReadOnly, LOCK_NOWAIT))
        return usage("Could not open file: " + source + ". Quitting.");
    while (!cache.Eof()) {
        int32_t which = 0; uint64_t blockNum = 0, transID = 0;
        cache >> which >> blockNum >> transID;
        theData[theData.getCount()] = CAcctCacheItem(blockNum, transID, which);
    }
    cache.Release();
    theData.Sort(sortByAccount);

    for (uint32_t i=0;i<theData.getCount();i++) {
        const CAccountWatch *item = &array[(uint32_t)theData[i].which];
        cout << theData[i] << "." << item->address << "." << item->color << item->name << cOff << "\n";
    }

    if (fileExists(source))
        removeFile(source);

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
            watch.color = convertColor(watch.color);
            if (!isAddress(watch.address)) {
                cerr << "invalid address " << watch.address << "\n";
                exit(0);
            }
            array[array.getCount()] = watch;
        }
    }
    return;
}

//-----------------------------------------------------------------------
void oneWatch(const CAccountWatch& watch) {
    cout << "    " << padRight(asStringU(watch.index)+".",4) << watch.address;
    SFString name = " (**" + watch.name.substr(0,12) + "&&)";
    size_t len = 20-name.length();
    cout << name.Substitute("**",watch.color).Substitute("&&",cOff);
    cout << SFString(' ', len);
}
