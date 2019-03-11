//TODO: This used to work: watch->nodeBal = getNodeBal(watch->balanceHistory, watch->address, watch->firstBlock-1);
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("~address_list",    "one or more addresses (0x...) to export"),
    COption("-fmt:<fmt>",       "export format (one of [json|txt|csv]"),
    COption("-fi(l)ter:<addr>", "show results for this address (you may specify more than one filter)"),
    COption("-useBlooms",       "use bloom filters to decide whether or not to re-check the cache"),
    COption("-ignoreDdos",      "ignore apparent dDos transactions."),
    COption("",                 "Export transactions for one or more Ethereum addresses.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern string_q cleanFmt(const string_q& str, export_t fmt);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    CAccountWatch::registerClass();

    export_t fmt = JSON;
    CStringArray filters;
    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
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

        } else if (startsWith(arg, "0x")) {
            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid address. Quitting...");
            CAccountWatch watch;
            watch.setValueByName("address", toLower(arg)); // don't change, sets bloom value also
            watch.setValueByName("name", toLower(arg));
            watch.extra_data = toLower("chifra/" + getVersionStr() + ": " + watch.address);
            watch.color = cBlue;
            watch.finishParse();
            monitors.push_back(watch);

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    if (monitors.size() == 0)
        return usage("You must provide at least one Ethereum address. Quitting...");

    // show certain fields and hide others
    manageFields(defHide, false);
    manageFields(defShow, true);

    CToml toml(monitors[0].address + ".toml");
    manageFields(toml.getConfigStr("fields", "hide", ""), false);
    manageFields(toml.getConfigStr("fields", "show", ""), true );

    // Try to articulate the watched addresses
    for (size_t i = 0 ; i < monitors.size() ; i++) {
        CAccountWatch *watch = &monitors[i];
        watch->abi_spec.loadAbiByAddress(watch->address);
        watch->abi_spec.loadAbiKnown("all");
        // We may as well articulate the named contracts while we're at it
        for (size_t n = 0 ; n < named.size() ; n++) {
            CAccountWatch *alt = &named.at(n);
            //if (alt->enabled)
            watch->abi_spec.loadAbiByAddress(alt->address);
        }
    }

    if (filters.size() > 0) {
        for (CAccountWatch& watch : monitors) {
            watch.enabled = false;
            for (auto const& addr : filters)
                if (addr % watch.address)
                    watch.enabled = true;
        }
    }

    if (fmt != JSON) {
        string_q defFmt = "[{DATE}]\t[{BLOCKNUMBER}]\t[{TRANSACTIONINDEX}]\t[{FROM}]\t[{TO}]\t[{VALUE}]\t[{ISERROR}]\t[{EVENTS}]";
        string_q format = toml.getConfigStr("formats", "trans_fmt", defFmt);
        if (format.empty())
            return usage("For non-json export a 'trans_fmt' string is required. Check your config file. Quitting...");
        expContext().fmtMap["trans_fmt"] = cleanFmt(format, fmt);
        format = toml.getConfigStr("formats", "trace_fmt", "{TRACES}");
        expContext().fmtMap["trace_fmt"] = cleanFmt(format, fmt);
        format = toml.getConfigStr("formats", "logentry_fmt", "{LOGS}");
        expContext().fmtMap["logentry_fmt"] = cleanFmt(format, fmt);
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    monitors.clear();
    showProgress = getGlobalConfig("acctExport")->getConfigBool("debug", "showProgress", false);
    useBloom = false;
    ignoreDdos = true;
    needsArt = false;
    needsTrace = false;

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {

    if (which == "options") {
        return substitute(str, "address_list", "<address> [address...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "[{addresses}] must start with '0x' and be forty two characters long.\n";
        return ret;
    }
    return str;
}

//-----------------------------------------------------------------------
string_q cleanFmt(const string_q& str, export_t fmt) {
    string_q ret = (substitute(substitute(substitute(str, "\n", ""), "\\n", "\n"), "\\t", "\t"));
    if (fmt == CSV)
        ret = "\"" + substitute(ret, "\t", "\",\"") + "\"";
    return ret;
}
