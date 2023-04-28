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
#include "options.h"

extern bool isBlockFinal(timestamp_t ts_block, timestamp_t ts_chain, timestamp_t distance);
extern void oneAppearance(const CAppearance& item, void* data);
//------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    // We want to get the latestBlock prior to turning on --prove for example
    COptions options;
    getBlockHeader(options.latestBlock, "latest");
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        string_q className = (options.traces ? GETRUNTIME_CLASS(CTrace)->m_ClassName
                                             : (options.logs ? GETRUNTIME_CLASS(CLog)->m_ClassName
                                                             : GETRUNTIME_CLASS(CBlock)->m_ClassName));
        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], className);

        if (options.traces) {
            options.blocks.forEveryBlockNumber(traceBlock, &options);

        } else if (options.logs) {
            options.handle_logs();

        } else {
            options.blocks.forEveryBlockNumber(visitBlock, &options);
        }
        once = false;
    }
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    etherlib_cleanup();
    return 0;
}

//------------------------------------------------------------
string_q doOneLightBlock(blknum_t num, const COptions& opt) {
    CBlock gold;
    getBlockLight(gold, num);
    if (gold.blockNumber == 0 && gold.timestamp == 0)
        gold.timestamp = blockZeroTs();
    HIDE_FIELD(CTransaction, "date");
    HIDE_FIELD(CTransaction, "ether");
    return gold.Format(expContext().fmtMap["format"]);
}

//------------------------------------------------------------
string_q doOneHeavyBlock(CBlock& gold, blknum_t num, const COptions& opt) {
    queryBlock(gold, uint_2_Str(num), true);
    if (gold.blockNumber == 0 && gold.timestamp == 0)
        gold.timestamp = blockZeroTs();
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
    if (opt.isRaw) {
        if (!queryRawBlock(result, uint_2_Str(num), true, opt.hashes)) {
            result = "Could not query raw block " + uint_2_Str(num) + ". Is an Ethereum node running?";

        } else {
            if (endsWith(result, "\n"))
                replaceReverse(result, "\n", "");

            // The query worked. If user wants us to write the block to cache, do so...
            if (opt.cache) {  // turn this on to force a write of the block to the disc
                CRPCResult generic;
                generic.parseJson3(result);
                result = generic.result;
                if (gold.parseJson3(result)) {
                    gold.finalized = isBlockFinal(gold.timestamp, opt.latestBlock.timestamp, opt.secsFinal);
                    writeBlockToBinary(gold, fileName);
                }
            }
        }
        opt.firstOut = false;

    } else if (opt.uncles && !opt.count) {
        uint64_t nUncles = getUncleCount(num);
        if (opt.count) {
            result = uint_2_Str(nUncles);
        } else {
            if (nUncles == 0) {
                // If we don't do this, we get extra commas
                opt.firstOut = true;
            } else {
                for (size_t i = 0; i < nUncles; i++) {
                    result += doOneUncle(num, i, opt);
                    if (i != (nUncles - 1)) {
                        if (!isText)
                            result += ",";
                        result += "\n";
                    }
                    opt.firstOut = false;
                }
                opt.firstOut = false;
            }
        }
    } else {
        if (opt.hashes || opt.count) {
            result = doOneLightBlock(num, opt);
        } else {
            result = doOneHeavyBlock(gold, num, opt);
            if (opt.cache) {  // turn this on to force a write of the block to the disc
                LOG2("writeBlockToBinary(" + uint_2_Str(gold.blockNumber) + ", " + fileName + ": " +
                     bool_2_Str(fileExists(fileName)));
                writeBlockToBinary(gold, fileName);
            }
        }
        opt.firstOut = false;
    }

    return result;
}

//------------------------------------------------------------
bool visitBlock(uint64_t num, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));
    if (!opt->firstOut) {
        if ((!isText && opt->filterType.empty()) || opt->count)
            cout << ",";
        cout << endl;
    }

    if (!opt->filterType.empty()) {
        opt->handle_appearances(num);

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
