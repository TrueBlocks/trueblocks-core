/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *-------------------------------------------------------------------------------------------*/
#include "acctlib.h"

//----------------------------------------------------------------
struct CIndexRecord {
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
    options.queryAddr = toLower(argv[1]);
    options.startBlock = str_2_Uint(argv[2]);

    string_q path = blockCachePath("addr_index/sorted_by_addr/");
    if (!folderExists(path)) {
        cerr << "Path for indexes (" << path << ") not found. Quitting..." << endl;
        return 1;
    }
    forEveryFileInFolder(path, visitFiles, &options);
    cerr << "Found " << options.items.size() << " transactions for account " << options.queryAddr << string_q(20, ' ') << "\n";
    for (auto const& item : options.items)
        cout << item << "\n";

    return 0;
}

//---------------------------------------------------------------
int findFunc(const void *v1, const void *v2) {
    const CIndexRecord *t1 = (const CIndexRecord *)v1;
    const CIndexRecord *t2 = (const CIndexRecord *)v2;
    return strncmp(t1->addr, t2->addr, 42);
}

//---------------------------------------------------------------
bool visitFiles(const string_q& path, void *data) {

    if (endsWith(path,'/')) {
        return forEveryFileInFolder(path+"*",visitFiles,data);

    } else {

        if (endsWith(path, ".txt")) {
            static int cnt = 0;
            if (!(++cnt%13)) {
                cerr << "Scanning " << path << "             \r";
                cerr.flush();
            }

            blknum_t bn = bnFromPath(path);
            Options *opt = (Options*)data;
            if (bn >= opt->startBlock) {
                address_t addr = opt->queryAddr;
                CMemMapFile blockFile(path, CMemMapFile::WholeFile, CMemMapFile::RandomAccess);
                CIndexRecord *records = (CIndexRecord *)(blockFile.getData());  // NOLINT
                uint64_t size = fileSize(path);
                uint64_t nRecs = size / 59;

                CIndexRecord t;
                strncpy(t.addr, addr.c_str(), 42);
                CIndexRecord *found = reinterpret_cast<CIndexRecord*>(bsearch(&t, records, nRecs, sizeof(CIndexRecord), findFunc));
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
                    uint64_t remains  = (pFound - pRecords) / sizeof(CIndexRecord);
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
