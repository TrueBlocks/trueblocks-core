/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
#include "options.h"

extern bool visitAddrs(blknum_t bn, blknum_t tx, blknum_t tc, const SFAddress& addr, void *data);
extern bool accumAddrs(blknum_t bn, blknum_t tx, blknum_t tc, const SFAddress& addr, void *data);
extern bool transFilter(const CTransaction *trans, void *data);
extern bool sortAddressArray(const CAddressItem& v1, const CAddressItem& v2);
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

    string_q sep(120, '-');
    cout << sep << "\n";
    if (options.testNum == 0) {

        cout << "Every address in block 4312145\n";
        block.forEveryAddress(visitAddrs, transFilter, NULL);

    } else if (options.testNum == 1) {

        cout << "Every unique addresses in block 4312145\n";
        block.forEveryUniqueAddress(visitAddrs, transFilter, NULL);

    } else if (options.testNum == 2) {

        cout << "Every unique addresses in block 4312145 (sorted)\n";
        vector<CAddressItem> array;
        block.forEveryUniqueAddress(accumAddrs, transFilter, &array);
        sort(array.begin(), array.end(), sortAddressArray);
        for (auto elem : array)
            cout << elem << "\n";
    } else {

        cout << "Testing JSON export of a block\n";
        CBlock block1;
        getBlock(block1, 22000);
        cout << sep << "\nUsing doExport\n" << sep << "\n";
        block1.doExport(cout) ;
        cout << sep << "\nUsing operator<<\n" << sep << "\n";
        cout << block1;
        cout << sep << "\nUsing Format\n" << sep << "\n";
        cout << block1.Format() << "\n";
        cout << sep << "\nUsing Format(fmt) - TODO: should report missing field, does not.\n" << sep << "\n";
        cout << block1.Format("[{BLOCKNUMBER}]\t[{HASH}]\t[{MINER}]\t[{NOT_A_FIELD}]") << "\n";
    }

    return 0;
}

//----------------------------------------------------------------
bool visitAddrs(blknum_t bn, blknum_t tx, blknum_t tc, const SFAddress& addr, void *data) {
    if (zeroAddr(addr))
        return true;

    CAddressItem item(bn, tx, tc, addr);
    cout << item << "\n";
    return true;
}

//----------------------------------------------------------------
bool accumAddrs(blknum_t bn, blknum_t tx, blknum_t tc, const SFAddress& addr, void *data) {
    if (zeroAddr(addr))
        return true;

    CAddressItem item(bn, tx, tc, addr);
    vector<CAddressItem> *array = (vector<CAddressItem> *)data;
    array->push_back(item);
    return true;
}

//----------------------------------------------------------------
bool transFilter(const CTransaction *trans, void *data) {
    // return true to filter (i.e. ignore) this transaction
    return (trans->transactionIndex == 2);
}

//----------------------------------------------------------------
bool sortAddressArray(const CAddressItem& v1, const CAddressItem& v2) {
    if (v1.blockNum != v2.blockNum)
        return v1.blockNum < v2.blockNum;
    int64_t vv1 = (int64_t)v1.transIndex;
    int64_t vv2 = (int64_t)v2.transIndex;
    if (vv1 != vv2)
        return vv1 < vv2;
    if (v1.traceId != v2.traceId)
        return v1.traceId < v2.traceId;
    return v1.addr < v2.addr;
}
