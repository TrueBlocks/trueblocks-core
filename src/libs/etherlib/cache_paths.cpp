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
#include <string>
#include "node.h"
#include "filenames.h"
#include "logfilter.h"

namespace qblocks {

//-------------------------------------------------------------------------
static string_q getFilename_local(cache_t type, const string_q& item1, const string_q& item2, const string_q& item3,
                                  bool asPath) {
    ostringstream os;
    switch (type) {
        case CT_BLOCKS:
            os << "blocks/";
            break;
        case CT_TXS:
            os << "txs/";
            break;
        case CT_TRACES:
            os << "traces/";
            break;
        case CT_RECONS:
            os << "recons/";
            break;
        case CT_NEIGHBORS:
            os << "neighbors/";
            break;
        default:
            ASSERT(0);  // should not happen
    }

    if (type == CT_RECONS) {
        string_q addr = toLower(substitute(item1, "0x", ""));
        string_q part1 = extract(addr, 0, 4);
        string_q part2 = extract(addr, 4, 4);
        string_q part3 = addr;
        replace(part3, part1, "");  // do not collapse
        replace(part3, part2, "");  // do not collapse
        os << part1 << "/" << part2 << "/" << part3 << "/";
        if (!asPath) {
            if (item2 != padNum9((uint64_t)NOPOS))
                os << item2 << "." << item3 << ".bin";
        }
    } else {
        os << extract(item1, 0, 2) << "/" << extract(item1, 2, 2) << "/" << extract(item1, 4, 2) << "/";
        if (!asPath) {
            os << item1;
            os << ((type == CT_TRACES || type == CT_TXS || type == CT_NEIGHBORS) ? "-" + item2 : "");
            os << (type == CT_TRACES && !item3.empty() ? "-" + item3 : "");
            os << ".bin";
        }
    }
    return getCachePath(os.str());
}

//-------------------------------------------------------------------------
string_q getBinaryCachePath(cache_t type, blknum_t bn, txnum_t txid, const string_q& trc_id) {
    return getFilename_local(type, padNum9(bn), padNum5(txid), trc_id, true);
}

//-------------------------------------------------------------------------
string_q getBinaryCacheFilename(cache_t type, blknum_t bn, txnum_t txid, const string_q& trc_id) {
    return getFilename_local(type, padNum9(bn), padNum5(txid), trc_id, false);
}

//-------------------------------------------------------------------------
string_q getBinaryCachePath(cache_t type, const address_t& addr, blknum_t bn, txnum_t txid) {
    return getFilename_local(type, addr, padNum9(bn), padNum5(txid), true);
}

//-------------------------------------------------------------------------
string_q getBinaryCacheFilename(cache_t type, const address_t& addr, blknum_t bn, txnum_t txid) {
    return getFilename_local(type, addr, padNum9(bn), padNum5(txid), false);
}

}  // namespace qblocks
