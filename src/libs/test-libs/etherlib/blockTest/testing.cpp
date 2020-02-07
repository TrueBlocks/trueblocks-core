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
#include "fromtransferfrom.h"
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
        getBlock(block, 4312145);

        cout << sep << endl;
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
    cout << item.Format(STR_DISPLAY_APPEARANCE) << endl;
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
    cout << "Every address in block 4312145" << endl;
    block.forEveryAddress(visitAddrs, transFilter, NULL);
}

//--------------------------------------------------------------
void everyUniqueAddress(CBlock& block) {
    cout << "Every unique addresses in block 4312145" << endl;
    block.forEveryUniqueAddress(visitAddrs, transFilter, NULL);
}

//--------------------------------------------------------------
void everySortedUniqueAddress(CBlock& block) {
    cout << "Every unique addresses in block 4312145 (sorted)" << endl;
    CAppearanceArray array;
    block.forEveryUniqueAddress(accumAddrs, transFilter, &array);
    sort(array.begin(), array.end(), sortAddressArray);
    for (auto elem : array)
        cout << elem.Format(STR_DISPLAY_APPEARANCE) << endl;
}

//--------------------------------------------------------------
void everyUniqueAddressPerTx(CBlock& block) {
    cout << "Every unique addresses per tx in block 4312145" << endl;
    block.forEveryUniqueAddressPerTx(visitAddrs, transFilter, NULL);
}

//--------------------------------------------------------------
void everySortedUniqueAddressPerTx(CBlock& block) {
    cout << "Every unique addresses per tx in block 4312145 (sorted)" << endl;
    CAppearanceArray array;
    block.forEveryUniqueAddressPerTx(accumAddrs, transFilter, &array);
    sort(array.begin(), array.end(), sortAddressArray);
    for (auto elem : array)
        cout << elem.Format(STR_DISPLAY_APPEARANCE) << endl;
}

//--------------------------------------------------------------
void testFormatting(CBlock& block) {
    QTransferFrom::registerClass();

    cout << "Testing JSON export of a block" << endl;
    CBlock block1;
    getBlock(block1, 22000);

    cout << sep << endl << "Using doExport" << endl << sep << endl;
    block1.doExport(cout);

    cout << sep << endl << "Using operator<<" << endl << sep << endl;
    cout << block1;

    cout << sep << endl << "Using Format" << endl << sep << endl;
    cout << block1.Format() << endl;

    verbose = 5;
    cout << sep << endl
         << "Using Format(fmt) - TODO(tjayrush): should report missing field, does not." << endl
         << sep << endl;
    cout << block1.Format("[{PARSED}]\t[{BLOCKNUMBER}]\t[{HASH}]\t[{MINER}]\t[{NOT_A_FIELD}]") << endl;

    QTransferFrom tf;
    tf._from = "0xTransferFromFrom";
    tf._to = "0xTransferFromTo";
    tf._value = 1001010100;
    tf.transactionIndex = 1002;
    cout << sep << endl << "Using Format(fmt) on a derived class QTransferFrom." << endl << sep << endl;
    cout << tf.Format("[{PARSED}]\t[{_FROM}]\t[{_TO}]\t[{TRANSACTIONINDEX}]\t[{NOT_A_FIELD}]") << endl;

    QFromTransferFrom ftf;
    cout << sep << endl << "Not registered, should error out" << endl << sep << endl;
    cout << ftf.Format("[{PARSED}]\t[{_FROM}]\t[{_TO}]\t[{TRANSACTIONINDEX}]\t[{NOT_A_FIELD}]") << endl;

    QFromTransferFrom::registerClass();
    ftf._from = "0xFromTransferFromFrom";
    ftf._to = "0xFromTransferFromTo";
    ftf.whop = "John";
    ftf.werp = "Jim";
    ftf._value = 121212121;
    ftf.transactionIndex = 1212;
    cout << sep << endl << "Using Format(fmt) on a derived class derived from QTransferFrom." << endl << sep << endl;
    cout << ftf.Format("[{PARSED}]\t[{_FROM}]\t[{_TO}]\t[{TRANSACTIONINDEX}]\t[{NOT_A_FIELD}]") << endl;
}
