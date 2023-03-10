/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
extern void everyUniqueAddressPerTx(CBlock& block);
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
                everyUniqueAddressPerTx(block);
                break;
            case 2:
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
const char* STR_DISPLAY_APPEARANCE_TESTING =
    "[{ADDRESS}]\t"
    "[{BLOCKNUMBER}]\t"
    "[{TRANSACTIONINDEX}]\t"
    "[{TRACEINDEX}]\t"
    "[{REASON}]\t"
    "[{NAME}]";

//----------------------------------------------------------------
bool visitAddrs(const CAppearance& item, void* data) {
    if (isZeroAddr(item.address))
        return true;
    cout << item.Format(STR_DISPLAY_APPEARANCE_TESTING) << "\n";
    return true;
}

//----------------------------------------------------------------
bool accumAddrs(const CAppearance& item, void* data) {
    if (isZeroAddr(item.address))
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
    if (v1.blockNumber != v2.blockNumber)
        return v1.blockNumber < v2.blockNumber;
    int64_t vv1 = (int64_t)v1.transactionIndex;
    int64_t vv2 = (int64_t)v2.transactionIndex;
    if (vv1 != vv2)
        return vv1 < vv2;
    if (v1.traceIndex != v2.traceIndex)
        return v1.traceIndex < v2.traceIndex;
    return v1.address < v2.address;
}

//--------------------------------------------------------------
void everyAddress(CBlock& block) {
    cout << "Every address in block 4312145\n";
    block.forEveryAppearanceInBlock(visitAddrs, transFilter, NULL);
}

//--------------------------------------------------------------
void everyUniqueAddressPerTx(CBlock& block) {
    cout << "Every unique addresses per tx in block 4312145\n";
    block.forEveryUniqueAppearanceInTxs(visitAddrs, transFilter, NULL);
}

//--------------------------------------------------------------
void everySortedUniqueAddressPerTx(CBlock& block) {
    cout << "Every unique addresses per tx in block 4312145 (sorted)\n";
    CAppearanceArray array;
    block.forEveryUniqueAppearanceInTxs(accumAddrs, transFilter, &array);
    sort(array.begin(), array.end(), sortAddressArray);
    for (auto elem : array) {
        cout << elem.Format(STR_DISPLAY_APPEARANCE_TESTING) << "\n";
    }
}

//--------------------------------------------------------------
void testFormatting(CBlock& block) {
}
