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
#include "options.h"

extern bool isBlockFinal(timestamp_t ts_block, timestamp_t ts_chain, timestamp_t distance);
//------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init(quickQuitHandler);

    // We want to get the latestBlock prior to turning on --prove for example
    COptions options;
    getBlock_light(options.latest, "latest");
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        // There can be more than one thing to do...
        if (options.filterType.empty())
            cout << (options.isMulti() ? "[" : "");

        string_q list = options.getBlockNumList();
        while (!list.empty() && !shouldQuit()) {
            blknum_t bn = str_2_Uint(nextTokenClear(list, '|'));
            if (!options.filterType.empty()) {
                getAddresses(bn, options);

            } else {
                string_q result = doOneBlock(bn, options);
                if (!result.empty()) {
                    cout << result;
                    if (!list.empty())
                        cout << ",";
                    cout << "\n";
                }
            }
        }

        if (options.filterType.empty())
            cout << (options.isMulti() ? "]" : "");
    }

    etherlib_cleanup();
    return 0;
}

//------------------------------------------------------------
string_q doOneBlock(uint64_t num, const COptions& opt) {

    CBlock gold;
    gold.blockNumber = num;
    string_q result;
    string_q numStr = uint_2_Str(num);
    if (opt.isRaw) {

        if (!queryRawBlock(result, numStr, true, opt.hashes_only)) {
            result = "Could not query raw block " + numStr + ". Is an Ethereum node running?";
        } else {
            if (opt.force) {  // turn this on to force a write of the block to the disc
                CRPCResult generic;
                generic.parseJson3(result);
                result = generic.result;
                if (gold.parseJson3(result)) {
                    string_q fileName = getBinaryCacheFilename(CT_BLOCKS, num);
                    gold.finalized = isBlockFinal(gold.timestamp, opt.latest.timestamp, opt.secsFinal);
                    writeBlockToBinary(gold, fileName);
                }
            }
        }

    } else {
        string_q fileName = getBinaryCacheFilename(CT_BLOCKS, gold.blockNumber);
        queryBlock(gold, numStr, (isApiMode() ? false : true));
        if (gold.blockNumber == 0 && gold.timestamp == 0)
            gold.timestamp = blockZeroTs;
        gold.finalized = isBlockFinal(gold.timestamp, opt.latest.timestamp, opt.secsFinal);

        if (opt.force) {  // turn this on to force a write of the block to the disc
            writeBlockToBinary(gold, fileName);
            LOG2("writeBlockToBinary(" + uint_2_Str(gold.blockNumber) + ", " + fileName + ": " + bool_2_Str(fileExists(fileName)));
        }

        result = gold.Format(opt.format);
    }

    return result;
}

//------------------------------------------------------------
void interumReport(ostream& os, blknum_t i) {
    os << (!(i%150) ? "." : (!(i%1000)) ? "+" : "");  // dots '.' at every 150, '+' at every 1000
    os.flush();
}

//------------------------------------------------------------
bool sortByBlocknumTxId(const CAppearance& v1, const CAppearance& v2) {
    if (v1.bn != v2.bn)
        return v1.bn < v2.bn;
    else if (v1.tx != v2.tx)
        return v1.tx < v2.tx;
    else if (v1.tc != v2.tc)
        return v1.tc < v2.tc;
    else if (v1.reason != v2.reason)
        return v1.reason < v2.reason;
    return v1.addr < v2.addr;
}

extern bool visitAddrs(const CAppearance& item, void *data);
extern bool transFilter(const CTransaction *trans, void *data);
//------------------------------------------------------------
string_q getAddresses(uint64_t num, const COptions& opt) {

    CBlock block;
    getBlock(block, num);
    if (opt.filterType == "uniq")
        block.forEveryUniqueAddress(visitAddrs, transFilter, (void*)&opt);
    else if (opt.filterType == "uniq_tx")
        block.forEveryUniqueAddressPerTx(visitAddrs, transFilter, (void*)&opt);
    else
        block.forEveryAddress(visitAddrs, transFilter, (void*)&opt);
    if (opt.count_only) {
        uint64_t cnt = opt.addrCounter;
        string_q be  = (cnt == 1 ? "was " : "were ");
        string_q adj = (contains(opt.filterType, "uniq") ? " unique" : "");
        cout << "There " << be << opt.addrCounter << adj << " addreses" << (cnt == 1 ? "" : "es") << " found in block " << block.blockNumber << ".\n";
        ((COptions*)&opt)->addrCounter = 0;
    }
    return "";
}

//----------------------------------------------------------------
bool visitAddrs(const CAppearance& item, void *data) {
    if (!isZeroAddr(item.addr)) {
        COptions *opt = (COptions*)data;
        if (item.tc != 10) {  // do not show or count the zeroTrace items since it duplicates the transaction's from and to
            if (opt->count_only) {
                opt->addrCounter++;

            } else {
                cout << item.Format(opt->format);
                if (!isTestMode())
                    cout << "                   ";
                cout << "\n";
                cout.flush();
            }
        }
    }
    return true;
}

//----------------------------------------------------------------
// Return 'true' if we want the caller NOT to visit the traces of this transaction
bool transFilter(const CTransaction *trans, void *data) {
    if (!ddosRange(trans->blockNumber))
        return false;
    return (getTraceCount(trans->hash) > 250);
}

//---------------------------------------------------------------------------
bool isBlockFinal(timestamp_t ts_block, timestamp_t ts_chain, timestamp_t distance) {
    // If the distance from the front of the node's current view of the front of the chain
    // is more than the number of seconds provided, consider the block final (even if it isn't
    // in a perfectly mathematical sense)
    return ((ts_chain - ts_block) > distance);
}
