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

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        if (once)
            cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], GETRUNTIME_CLASS(CBlock));
        options.blocks.forEveryBlockNumber(visitBlock, &options);
        once = false;
    }
    cout << exportPostamble(options.exportFmt, options.errors, expContext().fmtMap["meta"]);

    etherlib_cleanup();
    return 0;
}

//------------------------------------------------------------
string_q doOneBlock(uint64_t num, const COptions& opt) {

    string_q fileName = getBinaryCacheFilename(CT_BLOCKS, num);

    CBlock gold;
    gold.blockNumber = num;
    string_q result;
    if (opt.isRaw) {
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
                    gold.finalized = isBlockFinal(gold.timestamp, opt.latest.timestamp, opt.secsFinal);
                    writeBlockToBinary(gold, fileName);
                }
            }

        }

    } else {
        queryBlock(gold, uint_2_Str(num), (isApiMode() ? false : true));
        if (gold.blockNumber == 0 && gold.timestamp == 0)
            gold.timestamp = blockZeroTs;
        gold.finalized = isBlockFinal(gold.timestamp, opt.latest.timestamp, opt.secsFinal);
        if (opt.force) {  // turn this on to force a write of the block to the disc
            LOG2("writeBlockToBinary(" + uint_2_Str(gold.blockNumber) + ", " + fileName + ": " + bool_2_Str(fileExists(fileName)));
            writeBlockToBinary(gold, fileName);
        }
        result = gold.Format(expContext().fmtMap["format"]);

    }

    return result;
}

//----------------------------------------------------------------
bool visitAddrs(const CAppearance& item, void *data) {

    // We do not account for zero addresses or the addresses found in the zeroth trace since
    // it's identical to the transaction itself
    if (item.tc == 10 || isZeroAddr(item.addr))
        return !shouldQuit();

    COptions *opt = (COptions*)data;
    if (opt->count_only) {
        opt->addrCounter++;

    } else {
        cout << item.Format(expContext().fmtMap["format"]) << endl;

    }

    return !shouldQuit();
}

//----------------------------------------------------------------
// Return 'true' if we want the caller NOT to visit the traces of this transaction
bool transFilter(const CTransaction *trans, void *data) {
    if (!ddosRange(trans->blockNumber))
        return false;
    return (getTraceCount(trans->hash) > 250);
}

//------------------------------------------------------------
bool visitBlock(uint64_t num, void *data) {
    COptions *opt = reinterpret_cast<COptions *>(data);
    bool isText = (opt->exportFmt & (TXT1|CSV1));

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
            block.forEveryUniqueAddress(visitAddrs, transFilter, opt);
        else if (opt->filterType == "uniq_tx")
            block.forEveryUniqueAddressPerTx(visitAddrs, transFilter, opt);
        else
            block.forEveryAddress(visitAddrs, transFilter, opt);

        if (opt->count_only)
            cout << block.Format(substitute(substitute(expContext().fmtMap["format"], "[{ADDR_COUNT}]", uint_2_Str(opt->addrCounter)), "[{FILTER_TYPE}]", opt->filterType));

    } else {
        cout << doOneBlock(num, *opt);

    }

    opt->first = false;

    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool isBlockFinal(timestamp_t ts_block, timestamp_t ts_chain, timestamp_t distance) {
    // If the distance from the front of the node's current view of the front of the chain
    // is more than the number of seconds provided, consider the block final (even if it isn't
    // in a perfectly mathematical sense)
    return ((ts_chain - ts_block) > distance);
}


#if 0
int main(int argc, const char *argv[]) {
    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        if (once)
            cout << exportPreamble(options.exportFmt, expContext().fmtMap["header"], GETRUNTIME_CLASS(CLogEntry));
        forEveryTransactionInList(visitTransaction, &options, options.transList.queries);
        once = false;
    }
    cout << exportPostamble(options.exportFmt, options.errors, expContext().fmtMap["meta"]);
}

//--------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void *data) {

    //    COptions *opt = reinterpret_cast<COptions *>(data);
    //    bool isText = (opt->exportFmt & (TXT1|CSV1));

    if (contains(trans.hash, "invalid")) {
        string_q hash = nextTokenClear(trans.hash, ' ');
        opt->errors.push_back("Transaction " + hash + " not found.");
        return true; // continue even with an invalid item
    }

    if (opt->isRaw || opt->isVeryRaw) {
        string_q result;
        queryRawLogs(result, trans.blockNumber, trans.blockNumber);
        if (!isText && !opt->first)
            cout << ",";
        cout << result;
        opt->first = false;
        return true;
    }

    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////

    if (opt->articulate) {
        opt->abi_spec.loadAbiByAddress(trans.to);
        opt->abi_spec.articulateTransaction(&trans);
    }
    for (auto log : trans.receipt.logs) {
        if (opt->articulate) {
            opt->abi_spec.loadAbiByAddress(log.address);
            opt->abi_spec.articulateLog(&log);
        }
        manageFields("CFunction:message", !log.articulatedLog.message.empty());

        if (isText) {
            cout << trim(log.Format(expContext().fmtMap["format"]), '\t') << endl;
        } else {
            if (!opt->first)
                cout << ",";
            cout << "  ";
            incIndent();
            log.doExport(cout);
            decIndent();
            opt->first = false;
        }
    }
    return true;
}
#endif

#if 0
string_q result = doOneBlock(bn, options);
if (!result.empty()) {
    cout << result;
    if (!list.empty())
    cout << ",";
    cout << "\n";
    }
#endif
