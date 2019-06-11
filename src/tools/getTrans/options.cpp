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
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("~!trans_list",    "a space-separated list of one or more transaction identifiers (tx_hash, bn.txID, blk_hash.txID)"),
    COption("-articulate",     "articulate the transactions if an ABI is found for the 'to' address"),
    COption("-trace",          "display the transaction's trace"),
    COption("@fmt:<fmt>",      "export format (one of [none|json|txt|csv|api])"),
    COption("",                "Retrieve an Ethereum transaction from the local cache or a running node."),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_DISPLAY;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool noHeader = false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-t" || arg == "--trace") {
            incTrace = true;

        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;
            exportFmt = JSON1;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            string_q errorMsg;
            if (!wrangleTxId(arg, errorMsg))
                return usage(errorMsg);
            string_q ret = transList.parseTransList(arg);
            if (!ret.empty())
                return usage(ret);
        }
    }

    // Data wrangling
    if (!transList.hasTrans())
        return usage("Please specify at least one transaction identifier.");

    if (incTrace)
        SHOW_FIELD(CTransaction, "traces");

    if (isRaw || verbose)
        exportFmt = JSON1;

    if (isRaw) {
        string_q fields =
            "CBlock:blockHash,blockNumber|"
            "CTransaction:to,from,blockHash,blockNumber|"
            "CTrace|blockHash,blockNumber,subtraces,traceAddress,transactionHash,transactionPosition,type,error,articulatedTrace,action,result,date|"
            "CTraceAction:address,balance,callType,from,gas,init,input,refundAddress,to,value,ether|"
            "CTraceResult:address,code,gasUsed,output|";
        manageFields(fields, true);
    }

    if (articulate) {
        // show certain fields and hide others
        manageFields(defHide, false);
        manageFields(defShow, true);
        manageFields("CParameter:strDefault", false);  // hide
        manageFields("CTransaction:price", false);  // hide
        manageFields("CFunction:outputs", true);  // show
        manageFields("CTransaction:input", true);  // show
        manageFields("CLogEntry:topics", true);  // show
        abi_spec.loadAbiKnown("all");
    }

    // Display formatting
    string_q format;
    switch (exportFmt) {
        case NONE1:
        case TXT1:
        case CSV1:
            format = getGlobalConfig()->getConfigStr("display", "format", format.empty() ? STR_DISPLAY : format);
            if (incTrace)
                format += "\t[{TRACESCNT}]";
            manageFields("CTransaction:" + cleanFmt(format, exportFmt));
            break;
        case API1:
        case JSON1:
            format = "";
            break;
    }
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format, exportFmt);
    if (noHeader)
        expContext().fmtMap["header"] = "";

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RAW | OPT_OUTPUT);

    transList.Init();
    incTrace = false;
    articulate = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);
    Init();
    first = true;
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options") {
        return substitute(str, "trans_list", "<transID> [transID...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "[{trans_list}] is one or more space-separated identifiers which may be either a transaction hash,|"
                "a blockNumber.transactionID pair, or a blockHash.transactionID pair, or any combination.\n";
        ret += "This tool checks for valid input syntax, but does not check that the transaction requested exists.\n";
        ret += "This tool retrieves information from the local node or rpcProvider if configured "
                    "(see documentation).\n";
        ret += "If the queried node does not store historical state, the results may be undefined.\n";
        return ret;
    }
    return str;
}

//--------------------------------------------------------------------------------
const char* STR_DISPLAY =
"[{DATE}]\t"
"[{TIMESTAMP}]\t"
"[{BLOCKNUMBER}]\t"
"[{TRANSACTIONINDEX}]\t"
"[{HASH}]";

//--------------------------------------------------------------------------------
#ifdef BELONGS
    CAddressArray filters;
    bool belongs;
    bool chkAsStr;

////////////////////////////////////////////////////////////
    COption("@belongs:<addr>", "report true or false if the given address is found anywhere in the transaction"),
    COption("@asStrs",         "when checking --belongs, treat input and log data as a string"),

////////////////////////////////////////////////////////////
        } else if (arg == "--asStrs") {
            chkAsStr = true;

        } else if (startsWith(arg, "--belongs:")) {
            string_q orig = arg;
            arg = substitute(arg, "--belongs:", "");
            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid Ethereum address.\n");
            filters.push_back(str_2_Addr(toLower(arg)));

////////////////////////////////////////////////////////////
    if (chkAsStr && filters.size() == 0)
        return usage("chkAsStr only works with a --belongs filter.");

////////////////////////////////////////////////////////////
    if (options.filters.size() > 0) {
        bool on = options.chkAsStr;
        options.chkAsStr = false;
        forEveryTransactionInList(checkBelongs, &options, options.transList.queries);
        if (!options.belongs) {
            if (on) {
                options.chkAsStr = on;
                forEveryTransactionInList(checkBelongsDeep, &options, options.transList.queries);
            }
            if (!options.belongs) {
                for (auto addr : options.filters) {
                    cout << "\taddress " << cRed << addr << cOff << " not found ";
                    cout << options.transList.queries << "\n";
                }
            }
        }
    } else {

////////////////////////////////////////////////////////////
    filters.clear();
    belongs = false;
    chkAsStr = false;

////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------
bool visitAddrs(const CAppearance& item, void *data) {
    COptions *opt = (COptions*)data;

    if (opt->belongs)
        return false;

    for (auto addr : opt->filters) {
        if (addr % item.addr) {
            cout << "\t" << cGreen << " found at " << cTeal << item << cOff << "                     ";
            if (verbose)
                cout << "\n";
            else
                cout << "\r";
            cout.flush();
            opt->belongs = true;
            return true;  // we're done
        }
    }
    return true;
}

//--------------------------------------------------------------------------------
bool checkBelongs(CTransaction& trans, void *data) {
    // if we've been told we're done (because we found the target), stop searching
    if (!trans.forEveryAddress(visitAddrs, NULL, data))
        return false;
    return true;
}

//--------------------------------------------------------------------------------
bool checkBelongsDeep(CTransaction& trans, void *data) {
    COptions *opt = (COptions*)data;
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
#endif
