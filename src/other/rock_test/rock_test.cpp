//#define QBLOCKS
#define ROCKS
//#define FIXED

#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"
using namespace std;
using namespace rocksdb;

#include "etherlib.h"

string_q kDBPath = "./db";
#define N 200000
int main() {

#ifdef QBLOCKS
    CArchive out(WRITING_ARCHIVE);
    if (out.Lock("./file.bin", binaryWriteCreate, LOCK_WAIT)) {
        for (uint32_t i = 0 ; i < N ; i++) {
            string_q vs("value" + uint_2_Str(i));
            string_q ks("key" + uint_2_Str(i));
            out << ks << vs;
            cout << "in --> " << ks.c_str() << " : " << vs.c_str() << "\r";
            cout.flush();
        }
        out.Release();
    }

    CArchive in(READING_ARCHIVE);
    if (in.Lock("./file.bin", binaryReadOnly, LOCK_NOWAIT)) {
        while (!in.Eof()) {
            string_q ks, vs;
            in >> ks >> vs;
            cout << "out <-- " << ks.c_str() << " : " << vs.c_str() << "\r";
            cout.flush();
        }
        in.Release();
    }
    cout << "\n";
#endif

#ifdef FIXED
    char strs[N][20];
    memset(strs, '\0', N*20);
    for (uint32_t i = 0 ; i < N ; i++) {
        sprintf(&strs[i][0], "value: %s", uint_2_Str(i).c_str());
        cout << "in --> " << "key: " << i << " : " << strs[i] << "\r";
        cout.flush();
    }

    CArchive out(WRITING_ARCHIVE);
    if (out.Lock("./fixed.bin", binaryWriteCreate, LOCK_WAIT)) {
        out.Write(strs, 20, N);
        out.Release();
    }

    memset(strs, '\0', N*20);
    CArchive in(READING_ARCHIVE);
    if (in.Lock("./fixed.bin", binaryReadOnly, LOCK_NOWAIT)) {
        in.Read(strs, 20, N);
        in.Release();
    }

    for (uint32_t i = 0 ; i < N ; i++) {
        cout << "out <-- " << "key: " << i << " : " << strs[i] << "\r";
        cout.flush();
    }
    cout << "\n";
#endif

#ifdef ROCKS
    Options options;
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();
    options.create_if_missing = true;

    DB* db = NULL;
    Status s = DB::Open(options, kDBPath, &db);
    assert(s.ok());

    for (uint32_t i = 0 ; i < N ; i++) {
        ostringstream ks, vs;
        vs << "value " << i;
        ks << "key" << i;
        s = db->Put(WriteOptions(), ks.str(), vs.str());
        assert(s.ok());
        cout << "in --> " << ks.str().c_str() << " : " << vs.str().c_str() << "\r";
        cout.flush();
    }

    std::string value;
    for (uint32_t i = 0 ; i < N ; i++) {
        // get value
        ostringstream ks;
        ks << "key" << i;
        s = db->Get(ReadOptions(), ks.str(), &value);
        assert(s.ok());
        cout << "out <-- " << ks.str().c_str() << " : " << value << "\r";
    }
    cout << "\n";
    cout << "HERE\n";
    cout.flush();
    db->Close();
    cout << "HERE\n";
    cout.flush();
    if (db)
        delete db;
    cout << "HERE\n";
    cout.flush();
    cout << "HERE\n";
    cout.flush();
    cout << "HERE\n";
    cout.flush();
    cout << "HERE\n";
    cout.flush();
    cout << "HERE\n";
    cout.flush();
    cout << "HERE\n";
    cout.flush();
    cout << "HERE\n";
    cout.flush();
    cout << "HERE\n";
    cout.flush();
getchar();
#endif

    return 1;
}
