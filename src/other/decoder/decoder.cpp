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
#include "RLP.h"

//----------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    RLP rlp1("0x0000000000000000000000000000000000000000000000000000000000000002");
    RLP rlp2("0x0000000000000000000000000000000000000000000000000000000000000001");
    RLPs some;
    some.push_back(rlp1);
    some.push_back(rlp2);
    for (auto r : some)
        if (r) {
            cout << "actualSize: " << r.actualSize() << endl;
            cout << "isData: " << r.isData() << endl;
            cout << "isInt: " << r.isInt() << endl;
            cout << "isNull: " << r.isNull() << endl;
            cout << "isList: " << r.isList() << endl;
            cout << "size: " << r.size() << endl;
            cout << "string: " << r.toString() << endl;
        }
    return 1;
}

