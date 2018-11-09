//IT SHOULD NOT START IF IT CANNOT FIND THE FILE.
//I REMOVED AN ADDRESS ENTRY, THE FIRST, WHICH CHANGED THE NAME OF THE FILE
//SHOULD DISALLOW WEIRD COMMAND LINE options
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("-fmt:<fmt>",       "export format (one of [json|txt|csv]"),
    COption("-fi(l)ter:<addr>", "show results for this address (you may specify more than one filter)"),
    COption("-useBlooms",       "use bloom filters to decide whether or not to re-check the cache"),
    COption("-ignoreDdos",      "ignore apparent dDos transactions."),
    COption("",                 "Export transactions for one or more Ethereum addresses.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    CAccountWatch::registerClass();

    export_t fmt = JSON;
    CStringArray filters;
    if (!standardOptions(command))
        return false;

    Init();
#ifdef OUTPUT_REDIR
    outFile = "file.txt";
#endif
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        if (startsWith(arg, "-f:") || startsWith(arg, "--fmt:")) {

            arg = substitute(substitute(arg, "-f:", ""), "--fmt:", "");
                 if ( arg == "txt" ) fmt = TXT;
            else if ( arg == "csv" ) fmt = CSV;
            else if ( arg == "json") fmt = JSON;
            else return usage("Export format must be one of [ json | txt | csv ]. Quitting...");

        } else if (startsWith(arg, "-l:") || startsWith(arg, "--filter:")) {
            arg = substitute(substitute(arg, "-l:", ""), "--filter:", "");
            filters.push_back(arg);

        } else if (arg == "-i" || arg == "--ignoreDdos") {
            ignoreDdos = true;

        } else if ((arg == "-u") || (arg == "--useBlooms")) {
            useBloom = true;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    if (!fileExists("./config.toml"))
        return usage("The config.toml file was not found. Are you in the right folder? Quitting...\n");

    CToml toml("./config.toml");
    if (!loadWatches(toml))
        return false;

    if (filters.size() > 0) {
        for (CAccountWatch& watch : watches) {
            watch.enabled = false;
            for (auto addr : filters)
                if (addr % watch.address)
                    watch.enabled = true;
        }

//        HIDE_FIELD(CAccountWatch, "qbis");
//        for (CAccountWatch& watch : watches) {
//            cout << watch << "\n";
//        }
//        cout << "Press enter to continue >";
//        getchar();
    }

    // show certain fields and hide others
    manageFields(defHide, false);
    manageFields(defShow, true);
    manageFields(toml.getConfigStr("fields", "hide", ""), false);
    manageFields(toml.getConfigStr("fields", "show", ""), true );

#ifdef OUTPUT_REDIR
    if (!outFile.empty()) {
        outStream.open(outFile);
        out = new COutputPipe(outStream.rdbuf(), cout);
    }
#endif

    transFmt = "";  // empty string gets us JSON output
    if (fmt != JSON) {
        string_q format = toml.getConfigStr("formats", "trans_fmt", "");
        if (format.empty())
            return usage("Non-json export requires 'trans_fmt' string in config.toml. Quitting...");
        transFmt = cleanFmt(format);
        if (fmt == CSV)
            transFmt = "\"" + substitute(transFmt, "\t", "\",\"") + "\"";
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    transFmt = "";
    blk_minWatchBlock = 0;
    blk_maxWatchBlock = UINT32_MAX;
    showProgress = getGlobalConfig()->getConfigBool("debug", "showProgress", false);
    useBloom = false;
    ignoreDdos = true;
    needsArt = false;
    needsTrace = false;

    minArgs = 0;
#ifdef OUTPUT_REDIR
    if (out)
        delete out;
    out = NULL;
    outFile = "";
    outStream.close();
#endif
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
#ifdef OUTPUT_REDIR
    out = NULL;
#endif
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
#ifdef OUTPUT_REDIR
    if (out)
        delete out;
    outStream.close();
#endif
}

//-----------------------------------------------------------------------
string_q cleanFmt(const string_q& str) {
    return (substitute(substitute(substitute(str, "\n", ""), "\\n", "\n"), "\\t", "\t"));
}

//-----------------------------------------------------------------------
void manageFields(const string_q& listIn, bool show) {
    string_q list = substitute(listIn, " ", "");
    while (!list.empty()) {
        string_q fields = nextTokenClear(list, '|');
        string_q cl = nextTokenClear(fields, ':');
        CBaseNode *item =  createObjectOfType(cl);
        while (item && !fields.empty()) {
            string_q fieldName = nextTokenClear(fields, ',');
            if (fieldName == "all") {
                if (show) {
                    item->getRuntimeClass()->showAllFields();
                } else {
                    item->getRuntimeClass()->hideAllFields();
                }
            } else if (fieldName == "none") {
                if (show) {
                    item->getRuntimeClass()->hideAllFields();
                } else {
                    item->getRuntimeClass()->showAllFields();
                }
            } else {
                CFieldData *f = item->getRuntimeClass()->findField(fieldName);
                if (f)
                    f->setHidden(!show);
            }
        }
        delete item;
    }
}

//-----------------------------------------------------------------------
string_q defTransFmt = "{ \"date\": \"[{DATE}]\", \"from\": \"[{FROM}]\", \"to\": \"[{TO}]\", \"value\": \"[{VALUE}]\" }";
string_q defHide =
    "CTransaction: nonce, input"
"|" "CLogEntry: data, topics"
"|" "CTrace: blockHash, blockNumber, transactionHash, transactionPosition, traceAddress, subtraces"
"|" "CTraceAction: init"
"|" "CTraceResult: code"
"|" "CFunction: constant, payable, outputs, signature, encoding, type, articulate_str"
"|" "CParameter: type, indexed, isPointer, isArray, isObject";
string_q defShow =
    "CTransaction: price, gasCost, articulatedTx, traces, isError, date, ether"
"|" "CLogEntry: articulatedLog"
"|" "CTrace: articulatedTrace"
"|" "CTraceAction: "
"|" "CTraceResult: "
"|" "CFunction: "
"|" "CParameter: ";
