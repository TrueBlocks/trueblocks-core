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
#include "newbloom.h"

//----------------------------------------------------------------------
address_t randomAddress(void) {
    uint8_t bytes[20];
    for (size_t i = 0 ; i < 20 ; i++) {
        bytes[i] = (uint8_t)(RandomValue(0,16)*16 + RandomValue(0,16));
    }
    return bytes_2_Addr(bytes);
}

#define N_TESTS (3500/2)
//----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    cout << "d    nTotAddrs:     " << N_RAND_ADDR << endl;
    cout << "d    maxInserts:    " << MAX_INSERTS << endl;
    cout << "d    nBytes/bloom:  " << bloom_nt::BYTE_SIZE << " " << sizeof(bloom_nt) << endl;
    cout << "d    nBits/bloom:   " << bloom_nt::BIT_SIZE() << endl;

    CNewBloomArray blooms;
    CStringArray addrsInBloom, addrsNotInBloom;

    cerr << TIC() << endl;

    bool reading = fileExists("./bloom.bin");
    if (reading) {

        cerr << "m Reading addrs...";
        asciiFileToLines("./addresses.txt", addrsInBloom);
        asciiFileToLines("./notin.txt", addrsNotInBloom);
        cerr << TIC() << endl;

        cerr << "m Reading bloom...";
        CArchive file(READING_ARCHIVE);
        if (file.Lock("./bloom.bin", modeReadOnly, LOCK_NOWAIT)) {
            uint32_t size;
            file.Read(size);
            for (uint32_t b = 0 ; b < size ; b++) {
                uint32_t nI;
                file.Read(nI);

                bloom_nt bloom;
                bloom.nInserted = (size_t)nI;
                file.Read(bloom.bits, sizeof(uint8_t), bloom_nt::BYTE_SIZE);
                blooms.push_back(bloom);
            }
            file.Release();
        }
        cerr << TIC() << endl;

    } else {

        srand((unsigned int)time(NULL));

        cerr << "m Making addrs...";
        for (size_t i = 0 ; i < N_RAND_ADDR ; i++)
            addrsInBloom.push_back(randomAddress());
        ostringstream os;
        for (auto addr : addrsInBloom)
            os << addr << endl;
        stringToAsciiFile("./addresses.txt", os.str());
        for (size_t i = 0 ; i < N_RAND_ADDR ; i++)
            addrsNotInBloom.push_back(randomAddress());
        ostringstream os1;
        for (auto addr : addrsNotInBloom)
            os1 << addr << endl;
        stringToAsciiFile("./notin.txt", os1.str());
        cerr << TIC() << endl;

        cerr << "m Building blooms...";
        for (auto addr : addrsInBloom)
            addToSet(blooms, addr);
        cerr << "m Writing blooms...";
        CArchive file(WRITING_ARCHIVE);
        if (file.Lock("./bloom.bin", modeWriteCreate, LOCK_NOWAIT)) {
            file.Write((uint32_t)blooms.size());
            for (auto bloom : blooms) {
                file.Write((uint32_t)bloom.nInserted);
                file.Write(bloom.bits, sizeof(uint8_t), bloom_nt::BYTE_SIZE);
            }
            file.Release();
        }
        cerr << TIC() << endl;
    }

    size_t cnt = 0;
    cerr << "m Testing for inclusions...";
    for (auto addr : addrsInBloom) {
        cout << "y\t" << addr << "\t" << isMember(addr, blooms) << endl;
        if (++cnt >= N_TESTS)
            break;
    }
    cerr << TIC() << endl;

    cnt = 0;
    cerr << "m Testing for exclusions...";
    for (auto addr : addrsNotInBloom) {
        cout << "z\t" << addr << "\t" << isMember(addr, blooms) << endl;
        if (++cnt >= N_TESTS)
            break;
    }
    cerr << TIC() << endl;

    cout << "d    Size:          " << blooms.size() << endl;
    for (auto bloom : blooms) {
        cout << "d        nInserted: " << bloom.nInserted << endl;
        cout << "d        onBits:    " << bloom.nBitsHit();
        cout << " (" << (double(bloom.nBitsHit())/double(bloom_nt::BIT_SIZE()))*100. << "%)" << endl;
    }
    cout << "d    File:          " << "./bloom.bin (" << fileSize("./bloom.bin") << " bytes)" << endl;
    cout << fileSize("./shit") << endl;

    return 0;
}
