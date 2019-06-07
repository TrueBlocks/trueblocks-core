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

extern bool visitTransaction(CTransaction& trans, void *data);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        forEveryTransactionInList(visitTransaction, &options, options.transList.queries);
    }

    if (options.api_mode) {
        manageFields("CTrace:blockNumber,transactionPosition,traceAddress,error", true);
        manageFields("CTraceAction:callType,from,to,value,input,gas", true);
        manageFields("CTraceResult:gasUsed", true);
        for (auto item : options.items)
            cout << substitute(item.Format(STR_EXPORT_API), "null", " ") << endl;
        // In api mode we've been writing to a string stream (TODO(tjayrush): should write directly to the file)
        // so we need to close the redirect and start writing to cout again. We only write the location of the file.
        options.closeRedirect();
        cout << options.outputFn;

    } else {
        size_t cnt = 0;
        cout << "[";
        for (auto item : options.items) {
            if (cnt++ > 0)
                cout << ",";
            item.doExport(cout);
        }
        if (cnt && options.rawItems.size())
            cout << ",\n";

        cnt = 0;
        for (auto str : options.rawItems) {
            if (cnt++ > 0)
                cout << ",\n";
            cout << str;
        }
        cout << "]";
    }

    etherlib_cleanup();
    return 0;
}

//--------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void *data) {
    COptions *opt = (COptions*)data;
    if (contains(trans.hash, "invalid")) {
        ostringstream os;
        os << cRed << "{ \"error\": \"The item you requested (";
        os << nextTokenClear(trans.hash, ' ') << ") was not found.\" }" << cOff;
        opt->rawItems.push_back(os.str());
        return true;
    }

    if (!opt->isRaw) {
        if (opt->countOnly) {
            cout << trans.hash << "\t" << getTraceCount(trans.hash) << "\n";
        } else {
            CTraceArray traces;
            getTraces(traces, trans.getValueByName("hash"));
            for (auto tr : traces) {
                tr.pTrans = &trans;
                opt->items.push_back(tr);
            }
         }
        return true;
    }

    string_q fields =
        "CBlock:blockHash,blockNumber|"
        "CTransaction:to,from,blockHash,blockNumber|"
        "CTrace|blockHash,blockNumber,subtraces,traceAddress,transactionHash,transactionPosition,type,error,articulatedTrace,action,result,date|"
        "CTraceAction:address,balance,callType,from,gas,init,input,refundAddress,to,value,ether|"
        "CTraceResult:address,code,gasUsed,output|";
    manageFields(fields, true);

    string_q result;
    queryRawTrace(result, trans.getValueByName("hash"));
    if (opt->isVeryRaw) {
        opt->rawItems.push_back(result);
        return true;
    }
    CRPCResult generic;
    generic.parseJson3(result);
    CBlock bl;
    CTransaction tt; tt.pBlock = &bl;
    CReceipt receipt; receipt.pTrans = &tt;
    CTrace trace; trace.pTrans = &tt;
    trace.parseJson3(generic.result);
    opt->rawItems.push_back(trace.Format());
    return true;
}

//--------------------------------------------------------------
const char* STR_EXPORT_API =
"[{BLOCKNUMBER}]\t[{TRANSACTIONPOSITION}]\t[{TRACEADDRESS}]\t[{ACTION::CALLTYPE}]\t[{ERROR}]\t[{ACTION::FROM}]\t[{ACTION::TO}]\t[{ACTION::VALUE}]\t[{ACTION::GAS}]\t[{RESULT::GASUSED}]\t[{ACTION::INPUT}]";
