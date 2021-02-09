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
#define _DEBUG
#include "acctlib.h"
#include "options.h"

bool testDisplayStr(COptions& options);
//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        testDisplayStr(options);
    }

    return 0;
}

//--------------------------------------------------------------
bool testDisplayStr(COptions& options) {
    cout << string_q(30, '-') << options.className << string_q(30, '-') << endl;
    if (options.className == "CMonitor") {
        CMonitor monitor;
        SHOW_FIELD(CMonitor, "abi_spec");
        monitor.name = "monitor";

        CFunction func;
        func.name = "interface";
        func.type = "function";

        CParameter param;
        param.name = "input";
        func.inputs.push_back(param);
        param.name = "output";
        func.outputs.push_back(param);

        CAbi abi;
        abi.address = "0x1234567890123456789012345678901234567890";
        abi.loadAbiAddInterface(func);

        CEthState es;
        es.address = "0x1234567890123456789012345678901234567890";
        es.balance = str_2_Wei("6000000000000000000");
        es.nonce = 1001234;
        es.code = "0xTHIS_IS_BOGUS_CODE";
        es.storage = "0xTHIS_IS_BOGUS_STORAGE_DATA";
        monitor.stateHistory.push_back(es);

        CReconciliation is;
        is.amountIn = str_2_Wei("2000000000000000000");
        monitor.summaryStatement = is;

        cout << monitor.Format("[{p:STATEMENT::AMOUNTIN}]: [{STATEMENT::AMOUNTIN}]") << endl;
        // TODO(tjayrush): To address elements of array it should look like this:
        //      monitor.Format("[{ABI_SPEC::INTERFACES[0]::NAME}]" name of specific element
        // or
        //      monitor.Format("[{ABI_SPEC::INTERFACES[i]::NAME}]" name of all elements
        cout << abi.interfaces[0].Format("[{p:NAME}]: [{NAME}]") << endl;
        cout << abi.interfaces[0].inputs[0].Format("[{p:NAME}]: [{NAME}]") << endl;
        cout << monitor.stateHistory[0].Format("[{p:ADDRESS}]: [{ADDRESS}]") << endl;
        cout << monitor.stateHistory[0].Format("[{p:BALANCE}]: [{BALANCE}]") << endl;
        cout << monitor.stateHistory[0].Format("[{p:NONCE}]: [{NONCE}]") << endl;
        cout << monitor.stateHistory[0].Format("[{p:CODE}]: [{CODE}]") << endl;
        cout << monitor.stateHistory[0].Format("[{p:STORAGE}]: [{STORAGE}]") << endl;

    } else if (options.className == "CBlock") {
        CTraceResult tr;
        tr.code = "This is the code field of trace result";

        CTraceAction ta;
        ta.init = "This is the init field of the trace action";

        CTrace tra;
        tra.action = ta;
        tra.result = tr;
        tra.articulatedTrace = CFunction("artTrace");

        CLogEntry le;
        le.articulatedLog = CFunction("artLog");
        le.articulatedLog.encoding = "0xde8bc9f4";

        CReceipt r;
        r.contractAddress = "0x1234567890123456789012345678901234567890";
        r.logs.push_back(le);

        CBlock b;

        CTransaction t;
        SHOW_FIELD(CTransaction, "articulatedTx");
        t.articulatedTx.encoding = "0x861731d5";
        t.receipt = r;
        t.traces.push_back(tra);
        b.transactions.push_back(t);

        cout << tra.Format("[{p:RESULT::code}]: [{RESULT::code}]") << endl;
        cout << tra.Format("[{p:ACTION::inIT}]: [{ACTION::inIT}]") << endl;
        cout << tra.Format("[{p:ARTICULATEDTRACE::NAME}]: [{ARTICULATEDTRACE::NAME}]") << endl;
        cout << t.Format("[{p:RECEIPT::contractAddress}]: [{RECEIPT::contractAddress}]") << endl;
        cout << endl;

        cout << r.logs[0].Format("[{p:ARTICULATEDLOG::NAME}]: [{ARTICULATEDLOG::NAME}]") << endl;
        cout << b.transactions[0].Format("[{p:RECEIPT::contractAddress}]: [{RECEIPT::contractAddress}]") << endl;
        cout << b.transactions[0].Format("[{p:ARTICULATEDTX::encoding}]: [{ARTICULATEDTX::encoding}]") << endl;
        cout << b.transactions[0].receipt.logs[0].Format("[{p:ARTICULATEDLOG::ENCODING}]: [{ARTICULATEDLOG::ENCODING}]")
             << endl;
        cout << b.transactions[0].traces[0].Format("[{p:ACTION::init}]: [{ACTION::init}]") << endl;
        cout << endl;

        CTrace t1;
        cout << "Empty trace: " << t1 << endl;

    } else {
        cout << "Unknown node of type " << options.className << " not created.\n";
        return false;
    }

#if 0
    CBaseNode *node = getNode(options.className);
    if (!node) {
    } else {
        reportNode(node);
    }
    delete node;
#endif
    return true;
}
