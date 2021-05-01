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
#include "options.h"

//---------------------------------------------------------------------------
void COptions::handle_appearances(blknum_t num, void* data) {
    CBlock block;
    getBlock12(block, num);

    addrCounter = 0;
    if (filterType == "uniq")
        block.forEveryUniqueAppearanceInBlock(visitAddrs, transFilter, this);
    else if (filterType == "uniq_tx")
        block.forEveryUniqueAppearanceInBlockPerTx(visitAddrs, transFilter, this);
    else
        block.forEveryAppearanceInBlock(visitAddrs, transFilter, this);

    if (count) {
        string_q fmt = expContext().fmtMap["format"];
        fmt = substitute(fmt, "[{ADDR_COUNT}]", uint_2_Str(addrCounter));
        fmt = substitute(fmt, "[{FILTER_TYPE}]", filterType);
        if (verbose) {
            fmt = substitute(fmt, "[{UNCLE_COUNT}]", uint_2_Str(getUncleCount(num)));
            if (verbose > 2) {
                uint64_t cnt = 0;
                for (auto trans : block.transactions)
                    cnt += getTraceCount(trans.hash);
                fmt = substitute(fmt, "[{TRACE_COUNT}]", uint_2_Str(cnt));
            }
        }
        cout << block.Format(fmt);
    } else if (!nProcessed) {
        if (num == 0) {
            uint64_t cnt = 0;
            for (auto prefund : expContext().prefundMap) {
                CAppearance item;
                item.bn = num;
                item.tx = cnt++;
                item.addr = prefund.first;
                item.reason = "genesis";
                oneAppearance(item, data);
            }
        } else {
            // found no addresses -- i.e. early node software allowed misconfiguration of zero address miner
            CAppearance item;
            item.bn = num;
            item.tx = 99999;
            item.addr = "0x0000000000000000000000000000000000000000";
            item.reason = "miner";
            oneAppearance(item, data);
        }
    }
    first = false;
}

//----------------------------------------------------------------
void oneAppearance(const CAppearance& item, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));
    if (isText) {
        cout << trim(item.Format(expContext().fmtMap["format"]), '\t') << endl;
    } else {
        if (!opt->first)
            cout << ",";
        cout << "  ";
        indent();
        item.toJson(cout);
        unindent();
        opt->first = false;
    }
}

//----------------------------------------------------------------
bool visitAddrs(const CAppearance& item, void* data) {
    // We do not account for zero addresses or the addresses found in the zeroth trace since
    // it's identical to the transaction itself
    if (item.tc == 10 || isZeroAddr(item.addr))
        return !shouldQuit();

    COptions* opt = reinterpret_cast<COptions*>(data);
    opt->nProcessed++;
    if (opt->count) {
        opt->addrCounter++;

    } else {
        oneAppearance(item, data);
    }

    return !shouldQuit();
}

//----------------------------------------------------------------
// Return 'true' if we want the caller NOT to visit the traces of this transaction
bool transFilter(const CTransaction* trans, void* data) {
    if (!ddosRange(trans->blockNumber))
        return false;
    return (getTraceCount(trans->hash) > 250);
}
