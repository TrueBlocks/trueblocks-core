/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "acctlib.h"

//----------------------------------------------------------------
struct Thing1 {
public:
    char addr  [43];  // '0x' + 40 chars + \t
    char block [10];  // less than 1,000,000,000 + \t
    char txid  [6];   // less than 100000 + \n
};

//----------------------------------------------------------------
class Options {
public:
    address_t queryAddr;
    blknum_t  startBlock;
    CAcctCacheItemArray items;
    Options(void) { queryAddr = ""; startBlock = 0; }
};

extern bool visitFiles(const string_q& path, void *data);
//----------------------------------------------------------------
int main(int argc, char *argv[])
{
    colorsOff();
    if (argc != 3) {
        cerr << "You must provide an address and a starting block number. Quitting...\n";
        return 0;
    }

    Options options;
    options.queryAddr = argv[1];
    options.startBlock = str_2_Uint(argv[2]);

    string_q path = blockCachePath("addr_index/sorted_by_block/");
    forEveryFileInFolder(path, visitFiles, &options);
    cerr << "Found " << options.items.size() << " transactions for account " << options.queryAddr << string_q(20, ' ') << "\n";
    for (auto item : options.items)
        cout << item << "\n";

    return 0;
}

//---------------------------------------------------------------
int findFunc(const void *v1, const void *v2) {
    const Thing1 *t1 = (const Thing1 *)v1;
    const Thing1 *t2 = (const Thing1 *)v2;
    return strncmp(t1->addr, t2->addr, 42);
}

//---------------------------------------------------------------
bool visitFiles(const string_q& path, void *data) {

    if (endsWith(path,'/')) {
        return forEveryFileInFolder(path+"*",visitFiles,data);

    } else {

        if (endsWith(path, ".txt")) {
            cerr << "Scanning " << path << "             \r";
            cerr.flush();

            blknum_t bn = bnFromPath(path);
            Options *opt = (Options*)data;
            if (bn >= opt->startBlock) {
                address_t addr = opt->queryAddr;
                CMemMapFile blockFile(path, CMemMapFile::WholeFile, CMemMapFile::RandomAccess);
                Thing1 *records = (Thing1 *)(blockFile.getData());  // NOLINT
                uint64_t size = fileSize(path);
                uint64_t nRecs = size / 59;

                Thing1 t;
                strncpy(t.addr, addr.c_str(), 42);
                Thing1 *found = reinterpret_cast<Thing1*>(bsearch(&t, records, nRecs, sizeof(Thing1), findFunc));
                if (found) {
                    bool done = false;
                    while (found > records && !done) {
                        --found;
                        if (strncmp(found->addr, addr.c_str(), 42)) {
                            done = true;
                            found++;
                        }
                    }

                    uint64_t pRecords = (uint64_t)records;
                    uint64_t pFound   = (uint64_t)found;
                    uint64_t remains  = (pFound - pRecords) / sizeof(Thing1);
                    done = false;
                    for (uint64_t i = 0 ; i < remains && !done ; i++) {
                        char bl[10];
                        bzero(bl, 10);
                        strncpy(bl, found[i].block, 9);
                        char tx[6];
                        bzero(tx, 6);
                        strncpy(tx, found[i].txid, 5);
                        char ad[43];
                        bzero(ad, 43);
                        strncpy(ad, found[i].addr, 42);
                        if (!strncmp(addr.c_str(), found[i].addr, 42)) {
                            CAcctCacheItem item(str_2_Uint(bl), str_2_Uint(tx));
                            opt->items.push_back(item);
//                        } else {
//                            static int x=0;
//                            if (!(++x % 43)) {
//                                cerr << cTeal << bl << "." << tx << " " << cYellow << ad << cOff << "\r";
//                                cerr.flush();
//                            }
                        }
                        if (strncmp(ad, addr.c_str(), 42) > 0)
                            done = true;
                    }
                }
                blockFile.close();
            }
        }
    }
    return true;
}
