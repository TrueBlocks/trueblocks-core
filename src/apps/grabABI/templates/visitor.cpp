//
//  visitor.cpp
//  whiteHat
//
//  Created by Thomas Jay Rush on 3/8/18.
//  Copyright © 2018 com.GHC. All rights reserved.
//

#include "visitor.h"

//-----------------------------------------------------------------------
extern void loadWatches(const CToml& toml, CAccountWatchArray& array, const SFString& key, blknum_t& minny, blknum_t& maxxy);
blknum_t CVisitor::loadWatches(const CToml& toml) {

    blknum_t unused1 = 0, unused2 = 0;
    ::loadWatches(toml, named, "named", unused1, unused2);
    ::loadWatches(toml, watches, "list", blockStats.minWatchBlock, blockStats.maxWatchBlock);
    watches[watches.getCount()] = CAccountWatch(watches.getCount(), "Others", "Other Accts", 0, UINT32_MAX, bRed);
    return true;
}

//-----------------------------------------------------------------------
CAccountWatch *CVisitor::findWatch(SFAddress addr) {
    for (uint32_t i = 0 ; i < watches.getCount() ; i++)
        if (watches[i].address == addr)
            return &watches[i];
    return NULL;
}

//-----------------------------------------------------------------------
CAccountWatch *CVisitor::findWatch(uint32_t which) {
    if (which < watches.getCount())
        return &watches[which];
    return NULL;
}

//-----------------------------------------------------------------------
class CTemp {
public:
    uint64_t blockNum, transID;
    uint32_t whichWatch;
    CTemp(uint64_t bn, uint64_t tx, uint32_t w) : blockNum(bn), transID(tx), whichWatch(w) {}
};
typedef SFArrayBase<CTemp> SFTempArray;

//-----------------------------------------------------------------------
bool CVisitor::hasImport(void) {
    return fileExists("./cache/import.txt");
}

//-----------------------------------------------------------------------
uint32_t CVisitor::checkForImport(void) {
    if (!hasImport())
        return 0;

    SFTempArray imp;
    uint32_t cnt = 0;
    blknum_t currentLast = toLongU(asciiFileToString("./cache/lastBlock.txt"));
    SFString contents = asciiFileToString("./cache/import.txt");
    while (!contents.empty()) {
        SFString line  = nextTokenClear(contents,'\n');
        SFString bnStr = nextTokenClear(line, '\t');
        SFString txStr = nextTokenClear(line, '\t');
        SFString which = nextTokenClear(line, '\t');
        CTemp tmp(toUnsigned(bnStr), toUnsigned(txStr), (uint32_t)toUnsigned(which));
        CAccountWatch *watch = findWatch(tmp.whichWatch);
        if (watch) {
            cache << tmp.whichWatch << tmp.blockNum << tmp.transID;
            cache.flush();
            cout << "Imported: " << tmp.blockNum << "\t" << tmp.transID << "\r";
            cout.flush();
            if (tmp.blockNum > currentLast)
                stringToAsciiFile("./cache/lastBlock.txt", asStringU(tmp.blockNum) + "\r\n");
        }
    }
    removeFile("./cache/import.txt");
    // TODO(tjayrush) -- we need to de-dup here
    return cnt;
}
