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
#include "etherlib.h"
#include "options.h"

extern bool isBlockFinal(timestamp_t ts_block, timestamp_t ts_chain, timestamp_t distance);
//------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    // We want to get the latestBlock prior to turning on --prove for example
    COptions options;
    getBlock_light(options.latestBlock, "latest");
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        string_q className =
            (options.trace ? GETRUNTIME_CLASS(CTrace)->m_ClassName : GETRUNTIME_CLASS(CBlock)->m_ClassName);
        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], className);
        if (options.trace)
            options.blocks.forEveryBlockNumber(traceBlock, &options);
        else
            options.blocks.forEveryBlockNumber(visitBlock, &options);
        once = false;
    }
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    etherlib_cleanup();
    return 0;
}

//------------------------------------------------------------
string_q doOneLightBlock(blknum_t num) {
    CBlock gold;
    getBlock_light(gold, num);
    if (gold.blockNumber == 0 && gold.timestamp == 0)
        gold.timestamp = blockZeroTs;
    HIDE_FIELD(CTransaction, "datesh");
    HIDE_FIELD(CTransaction, "time");
    HIDE_FIELD(CTransaction, "date");
    HIDE_FIELD(CTransaction, "age");
    HIDE_FIELD(CTransaction, "ether");
    return gold.Format(expContext().fmtMap["format"]);
}

//------------------------------------------------------------
string_q doOneHeavyBlock(CBlock& gold, blknum_t num, const COptions& opt) {
    queryBlock(gold, uint_2_Str(num), (isApiMode() ? false : true));
    if (gold.blockNumber == 0 && gold.timestamp == 0)
        gold.timestamp = blockZeroTs;
    gold.finalized = isBlockFinal(gold.timestamp, opt.latestBlock.timestamp, opt.secsFinal);
    return gold.Format(expContext().fmtMap["format"]);
}

//------------------------------------------------------------
string_q doOneUncle(blknum_t num, blknum_t index, const COptions& opt) {
    CBlock gold;
    getUncle(gold, num, index);
    gold.finalized = isBlockFinal(gold.timestamp, opt.latestBlock.timestamp, opt.secsFinal);
    return gold.Format(expContext().fmtMap["format"]);
}

//------------------------------------------------------------
string_q doOneBlock(blknum_t num, COptions& opt) {
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));

    string_q fileName = getBinaryCacheFilename(CT_BLOCKS, num);
    CBlock gold;
    gold.blockNumber = num;
    string_q result;
    if (opt.isRaw || opt.isVeryRaw) {
        if (!queryRawBlock(result, uint_2_Str(num), true, opt.hashes_only)) {
            result = "Could not query raw block " + uint_2_Str(num) + ". Is an Ethereum node running?";

        } else {
            if (endsWith(result, "\n"))
                replaceReverse(result, "\n", "");

            // The query worked. If user wants us to write the block to cache, do so...
            if (opt.force) {  // turn this on to force a write of the block to the disc
                CRPCResult generic;
                generic.parseJson3(result);
                result = generic.result;
                if (gold.parseJson3(result)) {
                    gold.finalized = isBlockFinal(gold.timestamp, opt.latestBlock.timestamp, opt.secsFinal);
                    writeBlockToBinary(gold, fileName);
                }
            }
        }
        opt.first = false;

    } else if (opt.uncles) {
        uint64_t nUncles = getUncleCount(num);
        if (nUncles == 0) {
            // If we don't do this, we get extra commas
            opt.first = true;
        } else {
            for (size_t i = 0; i < nUncles; i++) {
                result += doOneUncle(num, i, opt);
                if (i != (nUncles - 1)) {
                    if (!isText)
                        result += ",";
                    result += "\n";
                }
                opt.first = false;
            }
            opt.first = false;
        }
    } else {
        if (opt.hashes_only) {
            result = doOneLightBlock(num);
        } else {
            result = doOneHeavyBlock(gold, num, opt);
            if (opt.force) {  // turn this on to force a write of the block to the disc
                LOG2("writeBlockToBinary(" + uint_2_Str(gold.blockNumber) + ", " + fileName + ": " +
                     bool_2_Str(fileExists(fileName)));
                writeBlockToBinary(gold, fileName);
            }
        }
        opt.first = false;
    }

    return result;
}

//----------------------------------------------------------------
bool visitAddrs(const CAppearance& item, void* data) {
    // We do not account for zero addresses or the addresses found in the zeroth trace since
    // it's identical to the transaction itself
    if (item.tc == 10 || isZeroAddr(item.addr))
        return !shouldQuit();

    COptions* opt = reinterpret_cast<COptions*>(data);
    if (opt->count) {
        opt->addrCounter++;

    } else {
        bool isText = (expContext().exportFmt & (TXT1 | CSV1));
        if (isText) {
            cout << trim(item.Format(expContext().fmtMap["format"]), '\t') << endl;
        } else {
            if (!opt->first)
                cout << ",";
            cout << "  ";
            incIndent();
            item.doExport(cout);
            decIndent();
            opt->first = false;
        }
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

//------------------------------------------------------------
bool visitBlock(uint64_t num, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));

    if (!opt->first) {
        if (!isText)
            cout << ",";
        cout << endl;
    }

    if (!opt->filterType.empty()) {
        CBlock block;
        getBlock(block, num);

        opt->addrCounter = 0;
        if (opt->filterType == "uniq")
            block.forEveryUniqueAppearanceInBlock(visitAddrs, transFilter, opt);
        else if (opt->filterType == "uniq_tx")
            block.forEveryUniqueAppearanceInBlockPerTx(visitAddrs, transFilter, opt);
        else
            block.forEveryAppearanceInBlock(visitAddrs, transFilter, opt);

        if (opt->count) {
            string_q fmt = expContext().fmtMap["format"];
            fmt = substitute(fmt, "[{ADDR_COUNT}]", uint_2_Str(opt->addrCounter));
            fmt = substitute(fmt, "[{FILTER_TYPE}]", opt->filterType);
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
        }
        opt->first = false;

    } else {
        cout << doOneBlock(num, *opt);
    }

    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool isBlockFinal(timestamp_t ts_block, timestamp_t ts_chain, timestamp_t distance) {
    // If the distance from the front of the node's current view of the front of the chain
    // is more than the number of seconds provided, consider the block final (even if it isn't
    // in a perfectly mathematical sense)
    return ((ts_chain - ts_block) > distance);
}
