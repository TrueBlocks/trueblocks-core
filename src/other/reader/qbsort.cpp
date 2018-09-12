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
class CItem {
    CItem(void) { }
public:
    blknum_t bn;
    blknum_t tx;
    hash_t   hash;
    CItem(const string_q& line) {
        hash = line;
        bn = str_2_Uint(nextTokenClear(hash, '\t'));
        tx = str_2_Uint(nextTokenClear(hash, '\t'));
    }
    friend bool operator<(const CItem& v1, const CItem& v2) {
        if (v1.bn < v2.bn)
            return true;
        if (v1.tx < v2.tx)
            return true;
        return v1.hash < v2.hash;
    }
};
typedef vector<CItem> CItemArray;

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CItem& item) {
    os << item.bn << "\t" << item.tx << "\t" << item.hash;
    return os;
}

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        CItemArray items;
        string_q contents = asciiFileToString(options.fileName);
        nextTokenClear(contents, '\n'); // skip line
        while (!contents.empty()) {
            string_q line = nextTokenClear(contents, '\n');
            if (!countOf(line, '\t'))
                return usage("Invalid line: " + line + "\n");
            items.push_back(CItem(line));
        }
        if (options.sort) {
            std::sort(items.begin(), items.end(), [](const CItem& lhs, const CItem& rhs) {
                uint64_t n1 = lhs.bn * 10000 + lhs.tx;
                uint64_t n2 = rhs.bn * 10000 + rhs.tx;
                return n1 < n2;
            });
        }

        cout << "blockNumber\ttransactionIndex\thash\n";
        for (auto item : items)
            cout << item << "\n";
    }
    return 0;
}
