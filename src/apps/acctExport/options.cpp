//TODO: This used to work: watch->nodeBal = getNodeBal(watch->balanceHistory, watch->address, watch->firstBlock-1);
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("~address_list",      "one or more addresses (0x...) to export"),
    COption("-fmt:<fmt>",         "export format (one of [json|txt|csv])"),
    COption("-articulate",        "articulate transactions, traces, logs, and outputs"),
    COption("@blocks:<on/off>",   "write blocks to the binary cache ('off' by default)"),
    COption("@txs:<on/off>",      "write transactions to the binary cache ('on' by default)"),
    COption("@t(r)aces:<on/off>", "write traces to the binary cache ('off' by default)"),
    COption("@ddos:<on/off>",     "skip over dDos transactions in export ('on' by default)"),
    COption("@maxTraces:<num>",   "if --ddos:on, the number of traces defining a dDos (default = 250)"),
    COption("",                   "Export full detail of transactions for one or more Ethereum addresses.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    CAccountWatch::registerClass();

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

        } else if (startsWith(arg, "-b") || startsWith(arg, "--blocks")) {
            arg = substitute(substitute(arg, "-b:", ""), "--blocks:", "");
            if (arg != "on" && arg != "off")
                return usage("Please provide either 'on' or 'off' for the --blocks options. Quitting...");
            writeBlocks = (arg == "on" ? true : false);

        } else if (startsWith(arg, "-t") || startsWith(arg, "--txs")) {
            arg = substitute(substitute(arg, "-t:", ""), "--txs:", "");
            if (arg != "on" && arg != "off")
                return usage("Please provide either 'on' or 'off' for the --txs options. Quitting...");
            writeTrxs = (arg == "on" ? true : false);

        } else if (startsWith(arg, "-r") || startsWith(arg, "--traces")) {
            arg = substitute(substitute(arg, "-r:", ""), "--traces:", "");
            if (arg != "on" && arg != "off")
                return usage("Please provide either 'on' or 'off' for the --trace options. Quitting...");
            writeTraces = (arg == "on" ? true : false);

        } else if (startsWith(arg, "-d") || startsWith(arg, "--ddos")) {
            arg = substitute(substitute(arg, "-d:", ""), "--ddos:", "");
            if (arg != "on" && arg != "off")
                return usage("Please provide either 'on' or 'off' for the --ddos options. Quitting...");
            skipDdos = (arg == "on" ? true : false);

        } else if (startsWith(arg, "-m") || startsWith(arg, "--maxTraces")) {
            arg = substitute(substitute(arg, "-m:", ""), "--maxTraces:", "");
            if (!isNumeral(arg))
                return usage("Please provide a number (you provided " + arg + ") for --maxTraces. Quitting...");
            maxTraces = str_2_Uint(arg);

        } else if (arg == "-a" || arg == "--articulate") {
            articulate = true;

        } else if (startsWith(arg, "0x")) {

            arg = toLower(arg);

            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid address. Quitting...");

            string_q fn = getMonitorPath(arg);
            if (!fileExists(fn)) {
                fn = (isTestMode() ? substitute(fn, getMonitorPath(""), "./") : fn);
                return usage("File not found '" + fn + ". Quitting...");
            }

            if (fileExists(fn + ".lck"))
                return usage("The cache lock file is present. The program is either already "
                             "running or it did not end cleanly the\n\tlast time it ran. "
                             "Quit the already running program or, if it is not running, "
                             "remove the lock\n\tfile: " + fn + ".lck'. Quitting...");

            //if (fileSize(fn) == 0)
            //    return usage("Nothing to export. Quitting...");

            CAccountWatch watch;
            // below - don't change, sets bloom value also
            watch.setValueByName("address", toLower(arg));
            // above - don't change, sets bloom value also
            watch.setValueByName("name", toLower(arg));
            watch.extra_data = getVersionStr() + "/" + watch.address;
            watch.color = cTeal;
            watch.finishParse();
            monitors.push_back(watch);

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    SHOW_FIELD(CTransaction, "traces");

    if (monitors.size() == 0)
        return usage("You must provide at least one Ethereum address. Quitting...");

    // show certain fields and hide others
    SEP4("default field hiding: " + defHide);
    manageFields(defHide, false);
    SEP4("default field showing: " + defHide);
    manageFields(defShow, true);

    CToml toml(getMonitorPath(monitors[0].address + ".toml"));
    SEP4("field hiding: " + toml.getConfigStr("fields", "hide", ""));
    manageFields(toml.getConfigStr("fields", "hide", ""), false);
    SEP4("field showing: " + toml.getConfigStr("fields", "show", ""));
    manageFields(toml.getConfigStr("fields", "show", ""), true );

    // Try to articulate the watched addresses
    for (size_t i = 0 ; i < monitors.size() ; i++) {
        CAccountWatch *watch = &monitors[i];
        watch->abi_spec.loadAbiByAddress(watch->address);
        watch->abi_spec.loadAbiKnown("all");
        string_q path = getMonitorPath(watch->address + ".toml");
        if (fileExists(path)) { // if there's a config file, let's use it
                                // user can tell us the names of other addresses
            CToml thisToml(path);
            string_q str = substitute(substitute(thisToml.getConfigJson("named", "list", ""),"[",""),"=",":");
            CAccountWatch item;
            while (item.parseJson3(str)) {
                item.address   = str_2_Addr(toLower(item.address));
                item.color     = convertColor(item.color);
                item.extra_data = getVersionStr() + "/" + item.address;
                item.finishParse();
                named.push_back(item);
                watch->abi_spec.loadAbiByAddress(item.address);
                item = CAccountWatch();
            }
        }
    }

    writeBlocks = getGlobalConfig("acctExport")->getConfigBool("settings", "writeBlocks", writeBlocks);;
    writeTrxs = getGlobalConfig("acctExport")->getConfigBool("settings", "writeTrxs", writeTrxs);;
    writeTraces = getGlobalConfig("acctExport")->getConfigBool("settings", "writeTraces", writeTraces);;
    skipDdos = getGlobalConfig("acctExport")->getConfigBool("settings", "skipDdos", skipDdos);;
    maxTraces = getGlobalConfig("acctExport")->getConfigBool("settings", "maxTraces", maxTraces);;

    if (fmt != JSON) {
        string_q defFmt = "[{DATE}]\t[{BLOCKNUMBER}]\t[{TRANSACTIONINDEX}]\t[{FROM}]\t[{TO}]\t[{VALUE}]\t[{ISERROR}]\t[{EVENTS}]";
        string_q format = toml.getConfigStr("formats", "trans_fmt", defFmt);
        if (format.empty())
            return usage("For non-json export a 'trans_fmt' string is required. Check your config file. Quitting...");
        expContext().fmtMap["transaction_fmt"] = cleanFmt(format, fmt);
        if (!contains(toLower(format), "traces"))
            HIDE_FIELD(CTransaction, "traces");

        format = toml.getConfigStr("formats", "trace_fmt", "{TRACES}");
        expContext().fmtMap["trace_fmt"] = cleanFmt(format, fmt);
        format = toml.getConfigStr("formats", "logentry_fmt", "{LOGS}");
        expContext().fmtMap["logentry_fmt"] = cleanFmt(format, fmt);
    }

    lastAtClient = getLastBlock_client();

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    monitors.clear();

    writeBlocks = false;
    writeTrxs = true;
    writeTraces = false;
    skipDdos = true;
    maxTraces = 250;
    fmt = JSON;
    articulate = false;

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
