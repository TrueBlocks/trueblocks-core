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

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CLogEntry));
        forEveryTransactionInList(visitTransaction, &options, options.transList.queries);
        once = false;
    }
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    etherlib_cleanup();
    return 0;
}

#if 0
//-------------------------------------------------------------------------
bool queryRawLogs1(string_q& results, hash_t hash, const address_t& addr, const CTopicArray& topics) {
    string_q data = "[{\"blockHash\":\"[HASH]\",\"address\":\"[ADDR]\"}]";
    replace(data, "[HASH]", hash);
    if (addr.empty())
        replace(data, ",\"address\":\"[ADDR]\"", "");
    else
        replace(data, "[ADDR]", addr_2_Str(addr));
    results = callRPC("eth_getLogs", data, true);
    return true;
}
bool queryRawLogs1(string_q& results, const hash_t& hash, const address_t& addr) {
    CTopicArray unused2;
    return queryRawLogs1(results, hash, addr, unused2);
}
bool queryRawLogs1(string_q& results, const hash_t& hash) {
    address_t unused1;
    CTopicArray unused2;
    return queryRawLogs1(results, hash, unused1, unused2);
}
//-------------------------------------------------------------------------
bool queryRawLogs1(string_q& results, uint64_t fromBlock, uint64_t toBlock, const address_t& addr,
                   const CTopicArray& topics) {
    string_q data = "[{\"fromBlock\":\"[START]\",\"toBlock\":\"[STOP]\",\"address\":\"[ADDR]\"}]";
    replace(data, "[START]", uint_2_Hex(fromBlock));
    replace(data, "[STOP]", uint_2_Hex(toBlock));
    if (addr.empty())
        replace(data, ",\"address\":\"[ADDR]\"", "");
    else
        replace(data, "[ADDR]", addr_2_Str(addr));
    results = callRPC("eth_getLogs", data, true);
    return true;
}
bool queryRawLogs1(string_q& results, uint64_t fromBlock, uint64_t toBlock, const address_t& addr) {
    CTopicArray unused2;
    return queryRawLogs1(results, fromBlock, toBlock, addr, unused2);
}
bool queryRawLogs1(string_q& results, uint64_t fromBlock, uint64_t toBlock) {
    address_t unused1;
    CTopicArray unused2;
    return queryRawLogs1(results, fromBlock, toBlock, unused1, unused2);
}
#endif

//--------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));

    if (contains(trans.hash, "invalid")) {
        string_q hash = nextTokenClear(trans.hash, ' ');
        opt->errors.push_back("Transaction " + hash + " not found.");
        return true;  // continue even with an invalid item
    }

    if (opt->isRaw || opt->isVeryRaw) {
        string_q result;
        queryRawLogs(result, trans.blockNumber, trans.blockNumber);
        if (!isText && !opt->first)
            cout << ",";
        cout << substitute(result, "mined", "");
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
