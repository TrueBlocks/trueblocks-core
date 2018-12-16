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
#define _DEBUG
#include "acctlib.h"
#include "options.h"

bool testDisplayStr(COptions& options);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    acctlib_init("binary", quickQuitHandler);

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
    if (options.className == "CAccountWatch") {

        CAccountWatch watch;
        SHOW_FIELD(CAccountWatch, "abi_spec");
        watch.name = "watch";

        CFunction func;
        func.name = "interface";

        CParameter param;
        param.name = "input";  func.inputs .push_back(param);
        param.name = "output"; func.outputs.push_back(param);

        CAbi abi;
        abi.address = "0x1234567890123456789012345678901234567890";
        abi.interfaces.push_back(func);
        watch.abi_spec = abi;

        CApiSpec api;
        api.method = "POST";
        watch.api_spec = api;

        CBalanceHistory bh;
        bh.balance = str_2_Wei("6000000000000000000");
        watch.balanceHistory.push_back(bh);

        CIncomeStatement is;
        is.inflow = str_2_Wei("2000000000000000000");
        watch.statement = is;

        cout << watch.Format("[{p:STATEMENT::INFLOW}]: [{STATEMENT::INFLOW}]") << endl;
        cout << watch.Format("[{p:API_SPEC::METHOD}]: [{API_SPEC::METHOD}]") << endl;
        cout << watch.Format("[{p:ABI_SPEC::ADDRESS}]: [{ABI_SPEC::ADDRESS}]") << endl;
        // TODO(tjayrush): To address elements of array it should look like this:
        //      watch.Format("[{ABI_SPEC::INTERFACES[0]::NAME}]" name of specific element
        // or
        //      watch.Format("[{ABI_SPEC::INTERFACES[i]::NAME}]" name of all elements
        cout << watch.abi_spec.interfaces[0].Format("[{p:NAME}]: [{NAME}]") << endl;
        cout << watch.abi_spec.interfaces[0].inputs[0].Format("[{p:NAME}]: [{NAME}]") << endl;
        cout << watch.balanceHistory[0].Format("[{p:BALANCE}]: [{BALANCE}]") << endl;

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

        CReceipt r;
        r.contractAddress = "0x1234567890123456789012345678901234567890";
        r.logs.push_back(le);

        CBlock b;

        CTransaction t;
        SHOW_FIELD(CTransaction, "articulatedTx");
        t.articulatedTx.encoding = "0x12345";
        t.receipt = r;
        t.traces.push_back(tra);
        b.transactions.push_back(t);

        cout << tra.Format("[{p:RESULT::code}]: [{RESULT::code}]") << endl;
        cout << tra.Format("[{p:ACTION::inIT}]: [{ACTION::inIT}]") << endl;
        cout << tra.Format("[{p:ARTICULATEDTRACE::NAME}]: [{ARTICULATEDTRACE::NAME}]") << endl;
        cout << t.Format("[{p:RECEIPT::contractAddress}]: [{RECEIPT::contractAddress}]") << endl;

        cout << r.logs[0].Format("[{p:ARTICULATEDLOG::NAME}]: [{ARTICULATEDLOG::NAME}]") << endl;
        cout << b.transactions[0].Format("[{p:RECEIPT::contractAddress}]: [{RECEIPT::contractAddress}]") << endl;
        cout << b.transactions[0].Format("[{p:ARTICULATEDTX::encoding}]: [{ARTICULATEDTX::encoding}]") << endl;
        cout << b.transactions[0].receipt.logs[0].Format("[{p:ARTICULATEDLOG::ENCODING}]: [{ARTICULATEDLOG::ENCODING}]") << endl;
        cout << b.transactions[0].traces[0].Format("[{p:ACTION::init}]: [{ACTION::init}]") << endl;

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

#if 0

class     = CBranch
baseClass = CTreeNode
fields    = string branchValue
includes  = etherlib.h|treenode.h
cIncs     = #include "treeroot.h"

class     = CInfix
baseClass = CTreeNode
fields    = CTreeNode *next
includes  = etherlib.h|treenode.h
cIncs     = #include "treeroot.h"

class     = CLeaf
baseClass = CTreeNode
fields    = CBlockNumArray blocks|uint64 counter
includes  = etherlib.h|treenode.h
cIncs     = #include "treeroot.h"

class    = CTreeNode
fields   = uint64 index|string prefixS
includes = etherlib.h
cIncs    = #include "treeroot.h"
scope    = extern

class    = CTreeRoot
fields   = CTreeNode *root
includes = etherlib.h|treenode.h|leaf.h|branch.h|infix.h

class = CAcctCacheItem
fields = uint64 blockNum|uint64 transIndex
includes = etherlib.h
sort = ((v1.blockNum != v2.blockNum) ? v1.blockNum < v2.blockNum : v1.transIndex < v2.transIndex)
equals = (blockNum == item.blockNum && transIndex == item.transIndex)

class     = CPriceQuote
fields    = timestamp timestamp|double close
includes  = abilib.h
cIncs     = #include "etherlib.h"|#include "pricesource.h"

#endif
