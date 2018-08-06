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
#include <algorithm>
#include <vector>
#define NOWALLETLIB
#include "acctlib.h"
// #include "fromtransferfrom.h"
#include "options.h"

//--------------------------------------------------------------
extern void everyAddress(CBlock& block);
extern void everyUniqueAddress(CBlock& block);
extern void everySortedUniqueAddress(CBlock& block);
extern void testFormatting(CBlock& block);
static string_q sep(120, '-');

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    COptions options;
    options.minArgs = 0;
    if (!options.prepareArguments(argc, argv))
        return 0;

    if (!options.parseArguments(options.commandList))
        return 0;

    CBlock block;
    getBlock(block, 4312145);

    cout << sep << "\n";
    switch (options.testNum) {
        case 0:  everyAddress(block);             break;
        case 1:  everyUniqueAddress(block);       break;
        case 2:  everySortedUniqueAddress(block); break;
        default: testFormatting(block);           break;
    }

    return 0;
}

//----------------------------------------------------------------
bool visitAddrs(const CAddressAppearance& item, void *data) {
    if (isZeroAddr(item.addr))
        return true;
    cout << item << "\n";
    return true;
}

//----------------------------------------------------------------
bool accumAddrs(const CAddressAppearance& item, void *data) {
    if (isZeroAddr(item.addr))
        return true;
    vector<CAddressAppearance> *array = (vector<CAddressAppearance> *)data;
    array->push_back(item);
    return true;
}

//----------------------------------------------------------------
bool transFilter(const CTransaction *trans, void *data) {
    // return true to filter (i.e. ignore) this transaction
    return (trans->transactionIndex == 2);
}

//----------------------------------------------------------------
bool sortAddressArray(const CAddressAppearance& v1, const CAddressAppearance& v2) {
    if (v1.getBn() != v2.getBn())
        return v1.getBn() < v2.getBn();
    int64_t vv1 = (int64_t)v1.getTx();
    int64_t vv2 = (int64_t)v2.getTx();
    if (vv1 != vv2)
        return vv1 < vv2;
    if (v1.getTc() != v2.getTc())
        return v1.getTc() < v2.getTc();
    return v1.addr < v2.addr;
}

//--------------------------------------------------------------
void everyAddress(CBlock& block) {
    cout << "Every address in block 4312145\n";
    block.forEveryAddress(visitAddrs, transFilter, NULL);
}

//--------------------------------------------------------------
void everyUniqueAddress(CBlock& block) {
    cout << "Every unique addresses in block 4312145\n";
    block.forEveryUniqueAddress(visitAddrs, transFilter, NULL);
}

//--------------------------------------------------------------
void everySortedUniqueAddress(CBlock& block) {
    cout << "Every unique addresses in block 4312145 (sorted)\n";
    vector<CAddressAppearance> array;
    block.forEveryUniqueAddress(accumAddrs, transFilter, &array);
    sort(array.begin(), array.end(), sortAddressArray);
    for (auto elem : array)
        cout << elem << "\n";
}

//--------------------------------------------------------------
void testFormatting(CBlock& block) {
}
