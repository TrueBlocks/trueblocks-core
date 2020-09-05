/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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

extern bool visitFile2(const string_q& path, void* data);
//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    CStringArray files;
	string_q x = substitute(indexFolder_blooms, "scraper.new", "scraper");
    cout << x << endl;
    forEveryFileInFolder(x, visitFile2, &files);
    forEveryFileInFolder(indexFolder_blooms, visitFile, &files);

    etherlib_cleanup();
    return 1;
}

size_t cnt = 0;
//----------------------------------------------------------------
bool visitFile(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", visitFile, data);

    } else {
        string_q p = ((CStringArray*)data)->operator[](cnt++);
        cout << path << endl;
        cout << p << endl;

        CBloomArray blooms1;
        readBloomFromBinary(path, blooms1);
        for (auto bloom : blooms1) {
            if (!bloom.isBitLit(0)) {
                cout << "\tzero nlit: " << bloom.nInserted << "\t\t" << bloom.nBitsHit() << string_q(80, ' ') << endl;
            } else {
                cout << "\tzero lit : " << bloom.nInserted << "\t\t" << bloom.nBitsHit() << string_q(80, ' ') << endl;
            }
        }

        CBloomArray blooms2;
        readBloomFromBinary(p, blooms2);
        for (auto bloom : blooms2) {
            if (!bloom.isBitLit(0)) {
                cout << "\tzero nli2: " << bloom.nInserted << "\t\t" << bloom.nBitsHit() << string_q(80, ' ') << endl;
            } else {
                cout << "\tzero lit2: " << bloom.nInserted << "\t\t" << bloom.nBitsHit() << string_q(80, ' ') << endl;
            }
        }

        cout << "\tsize 1: " << blooms1.size() << endl;
        cout << "\tsize 2: " << blooms2.size() << endl;

        // for (size_t i = 0 ; i < blooms1.size() ; i++) {
        //     if (blooms1[i] != blooms2[i])
        //         cout << "differ" << endl;
        //     else
        //         cout << "same" << endl;
        // }
    }
	cout << endl;
    return true;
}

//----------------------------------------------------------------
bool visitFile2(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", visitFile2, data);
    } else {
    CStringArray *f = (CStringArray*)data;
    cout << "adding: " << path << endl;
    f->push_back(path);
    }
    return true;
}
