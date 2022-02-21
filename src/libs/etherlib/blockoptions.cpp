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
#include "etherlib.h"
#include "blockoptions.h"

static uint64_t findBlockNumByHash(const hash_t& hash, void* data);
//--------------------------------------------------------------------------------
CBlockOptions::CBlockOptions(void) : blocks(nullptr) {
    Init();
    blocks.opts = this;
}

//--------------------------------------------------------------------------------
void CBlockOptions::Init(void) {
    blocks.hashFind = findBlockNumByHash;
}

//--------------------------------------------------------------------------------
uint64_t findBlockNumByHash(const hash_t& hash, void* data) {
    ASSERT(isHash(hash));
    CBlock block;
    getBlock(block, hash);  // getBlock returns true if it has transactions and false otherwise
    if (block.hash != hash) {
        LOG_WARN("Block hash '", hash, "' does not appear to be a valid block hash.");
        return NOPOS;
    }
    return block.blockNumber;
}

//--------------------------------------------------------------------------------
bool blockNumToString(uint64_t num, void* data) {
    if (num != NOPOS) {
        string_q* str = (string_q*)data;  // NOLINT
        *str += (uint_2_Str(num) + "|");
    }
    return true;
}

//--------------------------------------------------------------------------------
string_q CBlockOptions::getBlockNumList(void) {
    string_q ret;
    blocks.hashFind = findBlockNumByHash;
    blocks.forEveryBlockNumber(blockNumToString, &ret);
    return ret;
}

//--------------------------------------------------------------------------------
bool CHistoryOptions::needsHistory(void) const {
    blknum_t history_cnt = 250;  // used to be configurable, no longer
    return ((newestBlock - oldestBlock) >= history_cnt);
}

//--------------------------------------------------------------------------------
bool wrangleTxId(string_q& argOut, string_q& errorMsg) {
    if (contains(argOut, "0x"))
        return true;

    // valid args are 'latest', 'bn.txid', 'bn.txid.next', or 'bn.txid.prev'
    CStringArray parts;
    explode(parts, argOut, '.');

    if ((parts.size() == 2) && endsWith(argOut, ".*")) {
        CBlock block;
        getBlockLight(block, str_2_Uint(parts[0]));
        argOut = parts[0] + ".0";
        if (block.transactions.size() > 0)
            argOut += ("-to-" + uint_2_Str(block.transactions.size()));
        return true;
    }

    // txnum_t txid;
    if (parts.size() == 0 ||  // there are not enough
        (parts.size() == 1 &&
         (parts[0] != "latest" && parts[0] != "first")) ||  // there's only one and it's not 'latest'
        ((parts.size() == 2 || parts.size() == 3) &&
         (!isNumeral(parts[0]) || !isNumeral(parts[1]))) ||  // two or three, first two are not numbers
        parts.size() > 3) {                                  // too many
        errorMsg = argOut + " does not appear to be a valid transaction index.";
        return false;
    }

    if (parts.size() == 2)
        return true;

    // it's directional
    if (parts[0] == "latest") {
        CBlock block;
        getBlockLight(block, "latest");
        if (block.transactions.size() > 0) {
            ostringstream os;
            os << block.blockNumber << "." << (block.transactions.size() - 1);
            argOut = os.str();
            return true;
        }
        parts[0] = uint_2_Str(block.blockNumber);
        parts[1] = "0";
        parts[2] = "prev";
    }
    if (parts[0] == "first") {
        argOut = "46147.0";
        return true;
    }
    ASSERT(parts[2] == "prev" || parts[2] == "next");
    return getDirectionalTxId(str_2_Uint(parts[0]), str_2_Uint(parts[1]), parts[2], argOut, errorMsg);
}

//--------------------------------------------------------------------------------
bool getDirectionalTxId(blknum_t bn, txnum_t txid, const string_q& dir, string_q& argOut, string_q& errorMsg) {
    blknum_t lastBlock = getBlockProgress(BP_CLIENT).client;

    if (bn < firstTransactionBlock()) {
        argOut = uint_2_Str(firstTransactionBlock()) + ".0";
        return true;
    }

    CBlock block;
    getBlock(block, bn);

    argOut = "";
    txnum_t nextid = txid + 1;
    while (argOut.empty() && bn >= firstTransactionBlock() && bn <= lastBlock) {
        if (dir == "next") {
            if (nextid < block.transactions.size()) {
                argOut = uint_2_Str(block.blockNumber) + "." + uint_2_Str(nextid);
                return true;
            }
            block = CBlock();
            getBlock(block, ++bn);
            nextid = 0;
        } else if (dir == "prev") {
            if (txid > 0 && block.transactions.size() > 0) {
                argOut = uint_2_Str(block.blockNumber) + "." + uint_2_Str(txid - 1);
                return true;
            }
            if (bn == 0)
                return true;
            block = CBlock();
            getBlock(block, --bn);
            txid = block.transactions.size();
        }
    }
    errorMsg = "Could not find " + dir + " transaction to " + uint_2_Str(bn) + "." + uint_2_Str(txid);
    return false;
}

//--------------------------------------------------------------------------------
bool parseTransList2(COptionsBase* opt, COptionsTransList& transList, const string_q& argIn) {
    string_q errorMsg;
    string_q arg = argIn;
    if (!wrangleTxId(arg, errorMsg))
        return (opt ? opt->usage(errorMsg) : false);
    string_q ret = transList.parseTransList(arg);
    if (!ret.empty())
        return (opt ? opt->usage(ret) : false);
    return true;
}

//--------------------------------------------------------------
extern bool findTimestamp_binarySearch(CBlock& block, size_t first, size_t last);
bool findTimestamp_binarySearch(CBlock& block, size_t first, size_t last) {
    if (last > first) {
        size_t mid = first + ((last - first) / 2);
        CBlock b1, b2;
        getBlockHeader(b1, mid);
        getBlockHeader(b2, mid + 1);
        bool atMid = (b1.timestamp <= block.timestamp);
        bool atMid1 = (b2.timestamp <= block.timestamp);
        if (atMid && !atMid1) {
            block = b1;
            return true;
        } else if (!atMid) {
            // we're too high, so search below
            return findTimestamp_binarySearch(block, first, mid - 1);
        }
        // we're too low, so search above
        return findTimestamp_binarySearch(block, mid + 1, last);
    }
    getBlockHeader(block, first);
    return true;
}

//---------------------------------------------------------------
bool lookupDate(CBlock& block, const timestamp_t& ts, blknum_t latest) {
    time_q date = ts_2_Date(ts);

    // speed up
    blknum_t start = 1, stop = latest;
    if (date.GetYear() >= 2021) {
        start = 11565019;
    } else if (date.GetYear() >= 2020) {
        start = 9193265;
        stop = 11565019;
    } else if (date.GetYear() >= 2019) {
        start = 6988614;
        stop = 9193265;
    } else if (date.GetYear() >= 2018) {
        start = 4832685;
        stop = 6988614;
    } else if (date.GetYear() >= 2017) {
        start = 2912406;
        stop = 4832685;
    } else if (date.GetYear() >= 2016) {
        start = 778482;
        stop = 2912406;
    }

    block.timestamp = ts;
    bool ret = findTimestamp_binarySearch(block, start, stop);
    if (!isTestMode()) {
        cerr << "\r";
        cerr.flush();
    }
    return ret;
}

//--------------------------------------------------------------------------------
bool parseBlockList2(COptionsBase* opt, COptionsBlockList& blocks, const string_q& argIn, blknum_t latest) {
    string_q ret = blocks.parseBlockList_inner(argIn, latest);
    if (endsWith(ret, "\n")) {
        cerr << "\n  " << ret << "\n";
        return false;
    } else if (!ret.empty()) {
        return (opt ? opt->usage(ret) : false);
    }

    if (blocks.skip_type != BY_NOTHING) {
        time_q startDate = getBlockDate(blocks.start);
        time_q stopDate = getBlockDate(blocks.stop);
        CTimeArray times;
        switch (blocks.skip_type) {
            case BY_HOUR:
                expandHourly(times, startDate, stopDate);
                break;
            case BY_DAY:
                expandDaily(times, startDate, stopDate);
                break;
            case BY_WEEK:
                expandWeekly(times, startDate, stopDate);
                break;
            case BY_MONTH:
                expandMonthly(times, startDate, stopDate);
                break;
            case BY_QUARTER:
                expandQuarterly(times, startDate, stopDate);
                break;
            case BY_YEAR:
            default:
                expandAnnually(times, startDate, stopDate);
                break;
        }
        blocks.start = NOPOS;
        blocks.stop = NOPOS;
        for (auto t : times) {
            CBlock block;
            lookupDate(block, date_2_Ts(t), latest);
            blocks.numList.push_back(block.blockNumber);
        }
    }

    return true;
}

//--------------------------------------------------------------------------------
bool parseAddressList(COptionsBase* opt, CAddressArray& addrs, const string_q& argIn) {
    if (contains(argIn, ".eth")) {
        // We should never get this far with an unresolved .eth name unless there
        // is no address for this .eth name. Let the user know.
        return opt->usage("Invalid address '" + argIn + "'. Is this a valid .eth name?");
    } else if (!isAddress(argIn)) {
        return opt->usage("Invalid address '" + argIn + "'. Length (" + uint_2_Str(argIn.length()) +
                          ") is not equal to 40 characters (20 bytes).");
    }
    address_t lower = toLower(str_2_Addr(argIn));
    for (auto a : addrs) {
        if (a == lower) {
            LOG_WARN("Duplicate address. Not adding ", lower);
            return true;
        }
    }
    addrs.push_back(lower);
    return true;
}

//--------------------------------------------------------------------------------
bool parseTopicList2(COptionsBase* opt, CTopicArray& topics, const string_q& argIn) {
    if (!isHash(argIn))
        return opt->usage("Invalid topic '" + argIn + "'. Length (" + uint_2_Str(argIn.length()) +
                          ") is not equal to 64 characters (32 bytes).");
    topics.push_back(toLower(str_2_Topic(argIn)));
    return true;
}

//--------------------------------------------------------------------------------
bool parseFourbyteList(COptionsBase* opt, CFourbyteArray& fourbytes, const string_q& argIn) {
    if (!isHexStr(argIn))
        return opt->usage("Invalid value '" + argIn + "'.");
    if (!isFourbyte(argIn))
        return opt->usage("Invalid fourbyte '" + argIn + "'. Length (" + uint_2_Str(argIn.length()) +
                          ") is not equal to 8 characters (4 bytes).");
    fourbytes.push_back(toLower(str_2_Fourbyte(argIn)));
    return true;
}

//--------------------------------------------------------------------------------
bool parseStringList2(COptionsBase* opt, CStringArray& strings, const string& argIn) {
    strings.push_back(argIn);
    return true;
}
