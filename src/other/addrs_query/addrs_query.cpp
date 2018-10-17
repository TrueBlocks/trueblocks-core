/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

//----------------------------------------------------------------
struct Thing1 {
public:
    char addr[43];  // 0x + 40 chars + \0
    char num [8];   // less than 10,000,000 + \0
};

//----------------------------------------------------------------
class Options {
public:
    address_t addr;
    blknum_t num;
    CBlockNumArray blocks;
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

    Options opts;
    opts.addr = argv[1];
    opts.num = (str_2_Uint(argv[2]) / 10000) * 10000;

    string_q path = blockCachePath("addr_index/00/by_block/");
    forEveryFileInFolder(path, visitFiles, &opts);
    for (auto blk : opts.blocks)
        cout << "getBlock " << blk << " --addrs --filter " << opts.addr << " 2>/dev/null\n";

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
            blknum_t bn = bnFromPath(path);
            Options *opt = (Options*)data;
            if (bn >= opt->num) {
                address_t addr = opt->addr;
                CMemMapFile blockFile(path, CMemMapFile::WholeFile, CMemMapFile::RandomAccess);
                Thing1 *records = (Thing1 *)(blockFile.getData());  // NOLINT
                uint64_t size = fileSize(path);
                uint64_t nRecs = size / 51;

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
                        char n[8];
                        bzero(n, 8);
                        strncpy(n, found[i].num, 7);
                        char s[43];
                        bzero(s, 43);
                        strncpy(s, found[i].addr, 42);
                        if (!strncmp(addr.c_str(), found[i].addr, 42)) {
                            opt->blocks.push_back(str_2_Uint(n));
                        } else {
                            static int x=0;
                            if (!(++x%13)) {
                                cerr << cTeal << n << " " << cYellow << s << cOff << "\r";
                                cerr.flush();
                            }
                        }
                        if (strncmp(s, addr.c_str(), 42) > 0)
                            done = true;
                    }
                }
                blockFile.close();
            }
        }
    }
    return true;
}
