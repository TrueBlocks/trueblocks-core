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
#include <algorithm>
#include <vector>
#include "acctlib.h"
// #include "fromtransferfrom.h"
#include "options.h"

//--------------------------------------------------------------
extern void everyAddress(CBlock& block);
extern void everyUniqueAddress(CBlock& block);
extern void everyUniqueAddressPerTx(CBlock& block);
extern void everySortedUniqueAddress(CBlock& block);
extern void everySortedUniqueAddressPerTx(CBlock& block);
extern void testFormatting(CBlock& block);
static const string_q sep(120, '-');

//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    options.minArgs = 0;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        CBlock block;
        getBlock(block, 412149);

        cout << sep << "\n";
        switch (options.testNum) {
            case 0:
                everyAddress(block);
                break;
            case 1:
                everyUniqueAddress(block);
                everyUniqueAddressPerTx(block);
                break;
            case 2:
                everySortedUniqueAddress(block);
                everySortedUniqueAddressPerTx(block);
                break;
            default:
                testFormatting(block);
                break;
        }
    }
    return 0;
}

//----------------------------------------------------------------
bool visitAddrs(const CAppearance& item, void* data) {
    if (isZeroAddr(item.addr))
        return true;
    cout << item.Format(STR_DISPLAY_APPEARANCE) << "\n";
    return true;
}

//----------------------------------------------------------------
bool accumAddrs(const CAppearance& item, void* data) {
    if (isZeroAddr(item.addr))
        return true;
    CAppearanceArray* array = (CAppearanceArray*)data;  // NOLINT
    array->push_back(item);
    return true;
}

//----------------------------------------------------------------
bool transFilter(const CTransaction* trans, void* data) {
    // return true to filter (i.e. ignore) this transaction
    return (trans->transactionIndex == 2);
}

//----------------------------------------------------------------
bool sortAddressArray(const CAppearance& v1, const CAppearance& v2) {
    if (v1.bn != v2.bn)
        return v1.bn < v2.bn;
    int64_t vv1 = (int64_t)v1.tx;
    int64_t vv2 = (int64_t)v2.tx;
    if (vv1 != vv2)
        return vv1 < vv2;
    if (v1.tc != v2.tc)
        return v1.tc < v2.tc;
    return v1.addr < v2.addr;
}

//--------------------------------------------------------------
void everyAddress(CBlock& block) {
    cout << "Every address in block 4312145\n";
    block.forEveryAppearanceInBlock(visitAddrs, transFilter, NULL);
}

//--------------------------------------------------------------
void everyUniqueAddress(CBlock& block) {
    cout << "Every unique addresses in block 4312145\n";
    block.forEveryUniqueAppearanceInBlock(visitAddrs, transFilter, NULL);
}

//--------------------------------------------------------------
void everySortedUniqueAddress(CBlock& block) {
    cout << "Every unique addresses in block 4312145 (sorted)\n";
    CAppearanceArray array;
    block.forEveryUniqueAppearanceInBlock(accumAddrs, transFilter, &array);
    sort(array.begin(), array.end(), sortAddressArray);
    for (auto elem : array)
        cout << elem.Format(STR_DISPLAY_APPEARANCE) << "\n";
}

//--------------------------------------------------------------
void everyUniqueAddressPerTx(CBlock& block) {
    cout << "Every unique addresses per tx in block 4312145\n";
    block.forEveryUniqueAppearanceInBlockPerTx(visitAddrs, transFilter, NULL);
}

//--------------------------------------------------------------
void everySortedUniqueAddressPerTx(CBlock& block) {
    cout << "Every unique addresses per tx in block 4312145 (sorted)\n";
    CAppearanceArray array;
    block.forEveryUniqueAppearanceInBlockPerTx(accumAddrs, transFilter, &array);
    sort(array.begin(), array.end(), sortAddressArray);
    for (auto elem : array)
        cout << elem.Format(STR_DISPLAY_APPEARANCE) << "\n";
}

//--------------------------------------------------------------
void testFormatting(CBlock& block) {
}
