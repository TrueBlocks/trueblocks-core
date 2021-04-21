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
        monitor.name = "monitor";
        manageFields("CMonitor:all", FLD_SHOW);

        cout << monitor << endl;

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

    return true;
}
