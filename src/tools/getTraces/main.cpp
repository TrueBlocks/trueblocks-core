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
            cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CTrace));
        if (!options.filters.empty()) {
            for (auto f : options.filters) {
                CTraceArray traces;
                getTracesByFilter(traces, f);
                for (auto trace : traces) {
                    CBlock block;
                    getBlock(block, trace.blockNumber);
                    CTransaction trans;
                    trans.timestamp = block.timestamp;
                    trace.pTransaction = &trans;
                    cout << (!options.firstOut ? ", " : "");
                    cout << trace << endl;
                    options.firstOut = false;
                }
            }
        } else {
            forEveryTransaction(visitTransaction, &options, options.transList.queries);
        }
        once = false;
    }
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    etherlib_cleanup();
    return 0;
}

//--------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));

    if (contains(trans.hash, "invalid")) {
        string_q hash = nextTokenClear(trans.hash, ' ');
        opt->errors.push_back("transaction at " + hash + " returned an error: not found");
        return true;  // continue even with an invalid item
    }

    if (opt->isRaw) {
        string_q result;
        queryRawTrace(result, trans.getValueByName("hash"));
        if (!isText && !opt->firstOut)
            cout << ",";
        cout << result;
        opt->firstOut = false;
        return true;
    }

    //////////////////////////////////////////////////////
    if (opt->count) {
        uint64_t cnt = getTraceCount(trans.hash);
        if (isText) {
            cout << trans.blockNumber << "\t";
            cout << trans.transactionIndex << "\t";
            cout << trans.hash << "\t";
            cout << cnt << endl;
        } else {
            if (!opt->firstOut)
                cout << ",";
            cout << "{ ";
            cout << "\"bn\": \"" << trans.blockNumber << "\", ";
            cout << "\"tx_id\": \"" << trans.transactionIndex << "\", ";
            cout << "\"hash\": \"" << trans.hash << "\", ";
            cout << "\"count\": \"" << cnt << "\" }";
        }
        opt->firstOut = false;
        return true;
    }

    bool useCache = true;
    if (getEnvStr("NO_CACHE") == "true")
        useCache = false;
    loadTraces(trans, trans.blockNumber, trans.transactionIndex, useCache,
               (opt->skip_ddos && excludeTrace(&trans, opt->max)));

    //////////////////////////////////////////////////////

    if (opt->articulate)
        opt->abi_spec.loadAbiFromEtherscan(trans.to);

    for (auto trace : trans.traces) {
        if (!isTestMode() && isApiMode()) {
            qblocks::eLogger->setEndline('\r');
            LOG_INFO("Getting trace ", trans.blockNumber, ".", trans.transactionIndex, "-",
                     trace.getValueByName("traceAddress"), string_q(50, ' '));
            qblocks::eLogger->setEndline('\n');
        }

        if (opt->articulate) {
            opt->abi_spec.loadAbiFromEtherscan(trace.action.to);
            opt->abi_spec.articulateTrace(&trace);
        }
        manageFields("CFunction:message", !trace.articulatedTrace.message.empty());

        bool isSuicide = trace.action.selfDestructed != "";
        bool isCreation = trace.result.address != "";

        // do not colapse
        if (isCreation) {
            displayAsTrace(opt, trace);
            displayAsCreation(opt, trace);
        }
        // do not colapse
        if (isSuicide) {
            displayAsSuicide(opt, trace);
        }
        // do not colapse
        if (!isCreation && !isSuicide)
            displayAsTrace(opt, trace);
    }
    return true;
}

//--------------------------------------------------------------
bool displayAsCreation(COptions* opt, const CTrace& trace) {
    if (trace.result.address == "")
        return false;
    CTrace copy = trace;
    copy.action.from = "0x0";
    copy.action.to = trace.result.address;
    copy.action.callType = "creation";
    copy.action.value = trace.action.value;
    if (copy.traceAddress.size() == 0)
        copy.traceAddress.push_back("null");
    copy.traceAddress.push_back("s");
    copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
    copy.action.input = trace.action.input;
    return displayAsTrace(opt, copy);
}

//--------------------------------------------------------------
bool displayAsSuicide(COptions* opt, const CTrace& trace) {
    if (trace.action.refundAddress == "")
        return false;
    CTrace copy = trace;
    copy.action.from = trace.action.selfDestructed;
    copy.action.to = trace.action.refundAddress;
    copy.action.callType = "suicide";
    copy.action.value = trace.action.balance;
    copy.traceAddress.push_back("s");
    copy.transactionHash = uint_2_Hex(trace.blockNumber * 100000 + trace.transactionIndex);
    copy.action.input = "0x";
    return displayAsTrace(opt, copy);
}

//--------------------------------------------------------------
bool displayAsTrace(COptions* opt, const CTrace& trace) {
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));
    if (isText) {
        cout << trim(trace.Format(expContext().fmtMap["format"]), '\t') << endl;
    } else {
        if (!opt->firstOut)
            cout << ",";
        cout << "  ";
        indent();
        trace.toJson(cout);
        unindent();
        opt->firstOut = false;
    }
    return true;
}
