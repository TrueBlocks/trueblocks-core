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
#include <algorithm>
#include "etherlib.h"
#include "options.h"

extern bool visitTransaction(CTransaction& trans, void *data);
extern bool checkBelongs(CTransaction& trans, void *data);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    size_t cmdCnt = countOf(options.commandList, '\n') + 1;
    if (verbose && cmdCnt > 1)
        cout << "[";
    while (!options.commandList.empty()) {
        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;
        if (options.filters.size() > 0) {
            bool on = options.chkAsStr;
            options.chkAsStr = false;
            forEveryTransactionInList(checkBelongs, &options, options.transList.queries);
            if (!options.belongs) {
                if (on) {
                    options.chkAsStr = on;
                    forEveryTransactionInList(checkBelongs, &options, options.transList.queries);
                }
                if (!options.belongs) {
                    for (auto addr : options.filters) {
                        cout << "\taddress " << cRed << addr << cOff << " not found ";
                        cout << options.transList.queries << "\n";
                    }
                }
            }
        } else {
            size_t txCnt = countOf(options.transList.queries, '|') + 1;
            if (verbose && txCnt > 1 && cmdCnt < 2)
                cout << "[";
            forEveryTransactionInList(visitTransaction, &options, options.transList.queries);
            if (verbose && txCnt > 1 && cmdCnt < 2)
                cout << "]";
        }
    }
    if (verbose && cmdCnt > 1)
        cout << "]\n";
    cout.flush();
    return 0;
}

#define EARLY_QUIT 1
//----------------------------------------------------------------
bool visitAddrs(const CAddressAppearance& item, void *data) {
    COptions *opt = (COptions*)data;

#ifdef EARLY_QUIT
    if (opt->belongs)
        return false;
#endif

    for (auto addr : opt->filters) {
        if (addr % item.addr) {
            cout << "\t" << cGreen << " found at " << cTeal << item << cOff << "                     ";
#if 0
            if (contains(item.reason, "input") || contains(item.reason, "data") ||
                contains(item.reason, "generator") || (contains(item.reason, "trace") && contains(item.reason, "to")) ||
                (contains(item.reason, "log") && contains(item.reason, "topic"))
                )
                cout << "\r";
            else
                cout << "\n";
#else
            if (verbose)
                cout << "\n";
            else
                cout << "\r";
#endif
            cout.flush();
            opt->belongs = true;
#ifdef EARLY_QUIT
            return true;  // we're done
#else
            return false;
#endif
        }
    }
    return true;
}

//--------------------------------------------------------------
bool checkBelongs(CTransaction& trans, void *data) {
    COptions *opt = (COptions*)data;

    if (!trans.forEveryAddress(visitAddrs, NULL, data)) {
        return false; // if we've been told we're done (because we found the target), stop searching
    }

    // if we're still searching, and we want to search input and event data as a string, do so
    if (!opt->chkAsStr) {
        return true;
    }

    for (auto addr : opt->filters) {
        string_q bytes = substitute(addr, "0x", "");
        if (contains(trans.input, bytes)) {
            cout << "\t" << cRed << addr << cOff << " found at input by string search at ";
            cout << cTeal << trans.blockNumber << "." << trans.transactionIndex << cOff << "                       \n";
            opt->belongs = true;
            return false;
        }
        for (auto l : trans.receipt.logs) {
            if (contains(l.data, bytes)) {
                cout << "\t" << cRed << addr << cOff << " found at log by string search at ";
                cout << cTeal << trans.blockNumber << "." << trans.transactionIndex << cOff << "                       \n";
                opt->belongs = true;
                return false;
            }
        }
        getTraces(trans.traces, trans.hash);
        for (auto trace : trans.traces) {
            if (contains(trace.action.input, bytes)) {
                cout << "\t" << cRed << addr << cOff << " found at trace by string search at ";
                cout << cTeal << trans.blockNumber << "." << trans.transactionIndex << cOff << "                       \n";
                opt->belongs = true;
                return false;
            }
        }
    }
    return true;
}

//--------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void *data) {
    COptions *opt = reinterpret_cast<COptions *>(data);
    if (contains(trans.hash, "invalid")) {
        ostringstream os;
        os << cRed << "{ \"error\": \"The receipt for transaction ";
        os << nextTokenClear(trans.hash, ' ') << " was not found.\" }" << cOff;
        cerr << os.str();
        return true;
    }
    if (verbose && opt->index > 0)
        cout << ",";
    opt->index++;

    if (opt->isRaw) {
        // Note: this call is redundant. The transaction is already populated (if it's valid), but we need the raw data)
        string_q results;
        queryRawTransaction(results, trans.getValueByName("hash"));
        cout << results;
        return true;
    }

    string_q fmt = opt->format;
    if (opt->incTrace) {
        getTraces(trans.traces, trans.hash);
        SHOW_FIELD(CTransaction, "traces");
        fmt = substitute(fmt, "\n", "\tnTraces:[{TRACESCNT}]\n");
    }

    if (verbose) {
        if (opt->articulate) {
            opt->abi_spec.loadByAddress(trans.to);
            opt->abi_spec.articulateTransaction(&trans);
        }
        trans.doExport(cout);
    } else {
        cout << trans.Format(fmt);
    }

    return true;
}
