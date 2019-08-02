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
#include "etherlib.h"

extern void regenerate(void);
extern void check(void);
extern void truncate(void);
extern void move(void);
//----------------------------------------------------------------
int main(int argc, const char *argv[]) {

    cout << (fileSize(configPath("ts.bin")) / sizeof(uint32_t) / 2) << endl;

    size_t path = 1;
    switch (path) {
        case 0: regenerate(); break;
        case 1: check(); break;
        case 2: truncate(); break;
        case 3: move(); break;
        default: { }// nothing
    }
    return 1;
}

void regenerate(void) {
    freshenTimestampFile(getLastBlock_client());
}

void move(void) {
    string_q fn = configPath("ts.bin");

    CArchive file(READING_ARCHIVE);
    if (!file.Lock(fn, modeReadOnly, LOCK_NOWAIT))
        return;

    size_t ts_cnt = ((fileSize(fn) / sizeof(uint32_t)) / 2);
    uint32_t *ts_array = new uint32_t[ts_cnt * 2];  // blknum - timestamp
    if (!ts_array)
        return;

    file.Read(ts_array, sizeof(uint32_t) * 2, ts_cnt);
    file.Release();

}

void check(void) {
    string_q fn = configPath("ts.bin");

    CArchive file(READING_ARCHIVE);
    if (!file.Lock(fn, modeReadOnly, LOCK_NOWAIT))
        return;

    size_t ts_cnt = ((fileSize(fn) / sizeof(uint32_t)) / 2);
    uint32_t *ts_array = new uint32_t[ts_cnt * 2];  // blknum - timestamp
    if (!ts_array)
        return;

    file.Read(ts_array, sizeof(uint32_t) * 2, ts_cnt);
    file.Release();

    cout << "chain:\t";
    cout << 0 << "\t";
    cout << blockZeroTs << "\t";
    cout << "data:\t";
    cout << ts_array[0] << "\t";
    cout << ts_array[1] << "\t";
    cout << "eval:\t";
    cout << (0 == ts_array[0]) << "\t";
    cout << (blockZeroTs == ts_array[1]) << endl;

    blknum_t start = 8250000; //7264000; //2071099;
    #define skip 1
    //((blknum_t)RandomValue(11,221))
    for (blknum_t bn = start ; bn < ts_cnt ; bn = bn + skip) {
        CBlock block;
        getBlock_header(block, bn);
        cout << "chain:\t";
        cout << block.blockNumber << "\t";
        cout << block.timestamp << "\t";
        cout << "data:\t";
        cout << ts_array[bn*2] << "\t";
        cout << ts_array[bn*2+1] << "\t";
        cout << "eval:\t";
        bool b1 = block.blockNumber == ts_array[bn*2];
        bool b2 = block.timestamp == ts_array[bn*2+1];
        cout << (b1 ? greenCheck : redX) << "\t";
        cout << (b2 ? greenCheck : redX) << endl;
        if (!b1 || !b2)
            getchar();
    }
    file.Release();
    delete [] ts_array;
}

void truncate(void) {
    string_q fn = configPath("ts.bin");

    CArchive file(READING_ARCHIVE);
    if (!file.Lock(fn, modeReadOnly, LOCK_NOWAIT))
        return;
    size_t ts_cnt = ((fileSize(fn) / sizeof(uint32_t)) / 2);
    uint32_t *ts_array = new uint32_t[ts_cnt * 2];  // blknum - timestamp
    if (!ts_array)
        return;
    file.Read(ts_array, sizeof(uint32_t) * 2, ts_cnt);
    file.Release();

    ts_array[1] = blockZeroTs;
    if (!file.Lock(fn, modeWriteCreate, LOCK_WAIT))
        return;
    file.Write(ts_array, sizeof(uint32_t) * 2, ts_cnt); //8264850);
    file.Release();
    delete [] ts_array;

    check();
}
