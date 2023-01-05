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
#include "options.h"

extern bool visitPrefund(const Allocation& prefund, void* data);
//---------------------------------------------------------------------------
bool COptions::handle_appearances(blknum_t num) {
    CBlock block;
    getBlock(block, num);

    addrCounter = 0;
    if (filterType == "uniq")
        block.forEveryUniqueAppearanceInTxs(visitAddrs, transFilter, this);
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
            nPrefunds = 0;
            forEveryPrefund(visitPrefund, this);
        } else {
            // found no addresses -- i.e. early node software allowed misconfiguration of zero address miner
            CAppearance item;
            item.blockNumber = num;
            item.transactionIndex = 99999;
            item.address = "0x0000000000000000000000000000000000000000";
            item.reason = "miner";
            oneAppearance(item, this);
        }
    }
    firstOut = false;
    return true;
}

//----------------------------------------------------------------
bool shouldShow(const COptions* opt, const string_q& val) {
    if (opt->flow.empty()) {
        return true;
    }

    switch (opt->flow[0]) {
        case 't':  // to
            return contains(substitute(substitute(val, "_topic", ""), "generator", ""), "to");
        case 'f':  // from
            return contains(val, "from");
        case 'r':  // reward
            return contains(val, "miner") || contains(val, "uncle");
    }

    return true;
}

//----------------------------------------------------------------
void oneAppearance(const CAppearance& item, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));
    if (isText) {
        string_q val = trim(item.Format(expContext().fmtMap["format"]), '\t');
        if (shouldShow(opt, val)) {
            cout << val << endl;
        }
    } else {
        ostringstream os;
        indent();
        item.toJson(os);
        unindent();
        if (shouldShow(opt, os.str())) {
            if (!opt->firstOut)
                cout << ",";
            cout << "  ";
            cout << os.str();
            opt->firstOut = false;
        }
    }
}

//----------------------------------------------------------------
bool visitAddrs(const CAppearance& item, void* data) {
    // We do not account for zero addresses or the addresses found in the zeroth trace since
    // it's identical to the transaction itself
    if (item.traceIndex == 10 || isZeroAddr(item.address))
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
    if (!isDdos(trans->blockNumber))
        return false;
    return (getTraceCount(trans->hash) > 250);
}

//-----------------------------------------------------------------------
bool visitPrefund(const Allocation& prefund, void* data) {
    CAppearance item;
    item.blockNumber = 0;
    item.transactionIndex = ((COptions*)data)->nPrefunds++;
    item.address = prefund.address;
    item.reason = "genesis";
    oneAppearance(item, data);
    return true;
}
