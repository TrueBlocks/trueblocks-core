/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
// BEG_CODE_OPTIONS
static const COption params[] = {
//#define NEW_CODE
#ifdef NEW_CODE
	COption2("addr_list",	null,	"flag",	true,	true,	"one or more Ethereum addresses"),
	COption2("noBlooms",	null,	"flag",	false,	false,	"turn off bloom filters for performance testing"),
	COption2("staging",	null,	"flag",	false,	false,	"produce results in the staging folder instead of production folder"),
	COption2("unripe",	null,	"flag",	false,	false,	"visit unripe (not old enough and not yet staged or finalized) blocks"),
	COption2("daemon",	null,	"flag",	false,	false,	"we are being called in daemon mode which causes us to print results differently"),
	COption2("start",	null,	"<num>",	false,	false,	"first block to check"),
	COption2("",	null,	"flag",	false,	true,	"Index transactions for a given Ethereum address (or series of addresses).")
#else // NEW_CODE
    COption("~addr_list",       "one or more Ethereum addresses"),
    COption("@noBlooms",        "turn off bloom filters for performance testing"),
    COption("@staging",         "produce results in the staging folder instead of production folder"),
    COption("@unripe",          "visit unripe (not old enough and not yet staged or finalized) blocks"),
    COption("@daemon",          "we are being call in daemon mode which causes us to print results differently"),
    COption("",                 "Index transactions for a given Ethereum address (or series of addresses).\n"),
#endif
};
static const size_t nParams = sizeof(params) / sizeof(COption);
// END_CODE_OPTIONS

string_q dTabs;
bool daemonMode = false;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "-u" || arg == "--unripe") {
            visitTypes |= VIS_UNRIPE;

        } else if (arg == "-s" || arg == "--staging") {
            visitTypes |= VIS_STAGING;

        } else if (arg == "-n" || arg == "--noBlooms") {
            cerr << "Turning off blooms" << endl;
            useBlooms = false;

        } else if (arg == "-d" || arg == "--daemon") {
            daemonMode = true;

        } else if (startsWith(arg, "0x")) {
            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid address. Quitting...");

            // TODO(tjayrush): HARD CODED LIMIT???
            if (monitors.size() < 100) {
                CAccountWatch watch;
                watch.setValueByName("address", toLower(arg)); // don't change, sets bloom value also
                watch.setValueByName("name", toLower(arg));
                watch.extra_data = getVersionStr() + "/" + watch.address;
                watch.color = cBlue;
                watch.finishParse();
                monitors.push_back(watch);
            } else {
                return usage("You may scrape at most 30 addresses per invocation. Quitting...");
            }

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        } else {
            return usage("Invalid option: " + arg);
        }
    }

    if (monitors.size() == 0)
        return usage("You must provide at least one Ethereum address. Quitting...");

    if (getCurlContext()->nodeRequired) {
        if (!isParity() || !nodeHasTraces())
            return usage("This tool requires a running Parity node with --tracing enabled. Quitting...");
    }

    establishFolder(getMonitorPath("", FM_PRODUCTION));
    establishFolder(getMonitorPath("", FM_STAGING));
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_blooms);
    establishFolder(indexFolder_staging);
    establishFolder(indexFolder_ripe);

    dTabs = (daemonMode ? "\t  " : "");
    string_q endLine = (daemonMode ? "\r" : "\n");
    scanRange.first = UINT_MAX;
    for (auto monitor : monitors) {
        string_q fn1 = getMonitorPath(monitor.address);
        if (fileExists(fn1 + ".lck"))
            return usage("The cache file '" + fn1 + "' is locked. Quitting...");
        string_q fn2 = getMonitorLast(monitor.address);
        if (fileExists(fn2 + ".lck"))
            return usage("The last block file '" + fn2 + "' is locked. Quitting...");
        cerr << dTabs << "freshening: " << cYellow << monitor.address << cOff << "..." << endLine; cerr.flush();
        // If file doesn't exist, this will report '0'
        scanRange.first = min(scanRange.first, str_2_Uint(asciiFileToString(fn2)));
    }

    blknum_t unripe, ripe, staging, finalized, client;
    getLastBlocks(unripe, ripe, staging, finalized, client);

    scanRange.second = finalized;
    if (visitTypes & VIS_STAGING)
        scanRange.second = staging;
    if (visitTypes & VIS_UNRIPE)
        scanRange.second = unripe;

    // This would fail, for example, if the accounts are scraped further than the blocks (i.e. we
    // cleared the block index cache, but we didn't clear the account monitor cache
    if (scanRange.first >= scanRange.second) {
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    // We want to be able to run this more than once
    // optionOn(OPT_RUNONCE);
    // This app never actually writes to standard out, so we don't really need this
    // optionOn(OPT_OUTPUT);

    minArgs    = 0;
    visitTypes = VIS_FINAL;
    useBlooms  = true;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
    // just some cleanup of the screen
    if (!daemonMode)
        cerr << dTabs << "account scraper is finished.                 " << endl;
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {

    if (which == "options") {
        return substitute(str, "addr_list", "<address> [address...]");

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "[{addresses}] must start with '0x' and be forty two characters long.\n";
        return ret;
    }
    return str;
}

