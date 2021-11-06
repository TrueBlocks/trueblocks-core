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

//----------------------------------------------------------------
bool visitAddrs(const CAppearance& item, void* data) {
    if (item.tc == 10 || isZeroAddr(item.addr))
        return !shouldQuit();

    COptions* opt = reinterpret_cast<COptions*>(data);
    if (item.reason == "input" && item.addr != opt->accountedFor.address)
        return !shouldQuit();
    if (contains(item.reason, "topic") && item.addr != opt->accountedFor.address)
        return !shouldQuit();
    
    static blknum_t last = NOPOS;
    if (item.bn != last) {
        cout << endl;
        last = item.bn;
    }

    bool isText = (expContext().exportFmt & (TXT1 | CSV1));
    if (isText) {
        cout << trim(item.Format(expContext().fmtMap["format"]), '\t') << endl;
    } else {
        if (!opt->firstOut)
            cout << ",";
        cout << "  ";
        indent();
        item.toJson(cout);
        unindent();
        opt->firstOut = false;
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

//-----------------------------------------------------------------------
bool neighbors_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;
    trav->trans.forEveryUniqueAppearanceInTxPerTx(visitAddrs, transFilter, opt);
    prog_Log(trav, data);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
size_t neighbors_Count(CTraverser* trav, void* data) {
    return trav->trans.receipt.logs.size();
}
