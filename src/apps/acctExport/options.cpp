/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("-filt(e)r:<addr>", "display only transactions from the filter address"),
    COption("-fmt:<fmt>",       "export format (on of [json|txt|csv] (ignored if trans_fmt is non-empty)"),
    COption("",                 "Index transactions for a given Ethereum address (or series of addresses).\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

extern const char* defTransFmt;
extern string_q cleanFmt(const string_q& str);
extern void manageFields(const string_q& listIn, bool show);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        if (startsWith(arg, "-e:") || startsWith(arg, "--filter:")) {

            arg = substitute(substitute(arg, "-e:", ""), "--filter:", "");
            if (!isAddress(arg)) {
                cerr << usage("Invalid address " + arg + ". --filter flag requires a valid address. Quitting...");
                return false;
            }
            filter = arg;

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--fmt:")) {

            arg = substitute(substitute(arg, "-f:", ""), "--fmt:", "");
            if (arg != "json" && arg != "txt" && arg != "csv")
                return usage("Export format must be one of [ json | txt | csv ]. Quitting...");
            defaultFmt = arg;

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

    fmtStr     = cleanFmt(toml.getConfigStr ("formats", "trans_fmt", ""));
    hideFields = toml.getConfigStr ("fields",  "hide",      "");
    showFields = toml.getConfigStr ("fields",  "show",      "");

    // show certain fields and hide others
const char *defHide =
    "CTransaction: nonce, input"
"|" "CLogEntry: data, topics"
"|" "CTrace: blockHash, blockNumber, transactionHash, transactionPosition, traceAddress, subtraces"
"|" "CTraceAction: init"
"|" "CTraceResult: code"
"|" "CFunction: constant, payable, outputs"
"|" "CParameter: indexed, isPointer, isArray, isObject";

const char *defShow =
    "CTransaction: price, gasCost, articulatedTx, traces, isError, date, ether"
"|" "CLogEntry: articulatedLog"
"|" "CTraceAction: "
"|" "CTraceResult: "
"|" "CFunction: "
"|" "CParameter: ";

    manageFields(defHide, false);
    manageFields(defShow, true);
    manageFields(hideFields, false);
    manageFields(showFields, true);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    fmtStr = "";
    defaultFmt = ""; //"json";
    hideFields = "";
    showFields = "";
    filter = "";

    blk_minWatchBlock = 0;
    blk_maxWatchBlock = UINT32_MAX;

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
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
//const char* defTransFmt = "{ \"date\": \"[{DATE}]\", \"from\": \"[{FROM}]\", \"to\": \"[{TO}]\", \"value\": \"[{VALUE}]\" }";
const char* defTransFmt = "+=+ \"date\": \"[{DATE}]\", \"from\": \"[{FROM}]\", \"to\": \"[{TO}]\", \"value\": \"[{VALUE}]\" =+=";
