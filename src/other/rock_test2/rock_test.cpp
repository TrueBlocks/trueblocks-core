#define ROCKS

#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"
using namespace std;
using namespace rocksdb;

#include "acctlib.h"

#define ADD   true
//#define ADD   false
#define START 0
#define N     1000
bool visitAddress(blknum_t bn, blknum_t tx, blknum_t tc, const SFAddress& addr, void *data);
DB* db = NULL;
int main() {

    std::string kDBPath = "./db";
    etherlib_init();

    Options options;
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();
    options.create_if_missing = true;

    Status s = DB::Open(options, kDBPath, &db);
    assert(s.ok());

    if (ADD) {
        for (blknum_t b = START ; b < START + N ; b++) {
            CBlock block;
            getBlock(block, b);
            block.forEveryAddress(visitAddress, NULL, &block);
        }
    }

    uint32_t cnt = 0;
    Iterator* it = db->NewIterator(ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        string key = it->key().ToString();
        string value = it->value().ToString();
        cout << cnt++ << ". " << key << " : " << value << endl;
    }
    assert(it->status().ok()); // Check for any errors found during the scan
    delete it;
    cout << "Done...\n"; cout.flush();
    delete db;
    cout << "Done...\n"; cout.flush();
    return 0;
}

bool visitAddress(blknum_t bn, blknum_t tx, blknum_t tc, const SFAddress& addr, void *data) {
    int32_t xx = (int32_t)tx;
    if (tx == NOPOS)
        xx = -1;
    string key   = addr.c_str();
    string value;
    Status s = db->Get(ReadOptions(), key, &value);
    if (value.empty()) {
        ostringstream os;
        os << bn;
        value = os.str();
        s = db->Put(WriteOptions(), key, value);
        assert(s.ok());
        cout << "add  --> " << bn << "." << xx << " : " << cTeal   << key << " | " << value << cOff << "\n";
    } else {
        cout << "skip --> " << bn << "." << xx <<  ": " << cYellow << key << " | " << value << cOff << "\r";
    }
    cout.flush();
    return true;
}
