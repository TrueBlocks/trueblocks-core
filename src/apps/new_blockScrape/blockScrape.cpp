/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-------------------------------------------------------------------------
inline bool sortByBlockNum(const string_q& v1, const string_q& v2) {
    blknum_t b1 = str_2_Uint(v1.substr(v1.find("\t")));
    blknum_t b2 = str_2_Uint(v2.substr(v2.find("\t")));
    return b1 < b2;
}
class CBlockIndexItem {
public:
    uint32_t bn;
    uint32_t ts;
    uint16_t tx;
};
typedef vector<CBlockIndexItem> CBlockIndex;
bool operator<(const CBlockIndexItem& i1, const CBlockIndexItem& i2) {
    return i1.bn < i2.bn;
}
//----------------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init("local",  defaultQuitHandler);

    string_q path = "/Volumes/Samsung_T7/scraper_testing/no_txs.txt";
    string_q contents;
    cerr << "reading..." << endl;
    asciiFileToString(path, contents);
    CStringArray array;
    array.reserve(10000000);
    explode(array, contents, '\n');
    CBlockIndex index;
    cerr << "parsing..." << endl;
    for (auto a : array) {
        CBlockIndexItem item;
        item.bn = (uint32_t)strtoul(nextTokenClear(a, '\t').c_str(),NULL,10);
        item.ts = (uint32_t)strtoul(nextTokenClear(a, '\t').c_str(),NULL,10);
        item.tx = (uint16_t)strtoul(nextTokenClear(a, '\t').c_str(),NULL,10);
        if (item.bn < 7360324)
            index.push_back(item);
    }

    cerr << "sorting..." << endl;
    sort(index.begin(), index.end());

    cerr << "writing..." << endl;
    CArchive output(WRITING_ARCHIVE);
    output.Lock(substitute(path, "no_txs.txt", "fullBlocks.bin"), modeWriteCreate, LOCK_NOWAIT);
    for (auto i : index)
        output << i.bn << i.ts << i.tx;
    output.Release();

#if 0
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        cerr << bGreen << "Scraping new blocks..." << "\n" << cOff;
        if (!handle_freshen(options))
            cerr << "\tThe tool ended with an error.";
        cerr << bGreen << "...done\n" << cOff;
    }
#endif

    etherlib_cleanup();
    return 0;
}
