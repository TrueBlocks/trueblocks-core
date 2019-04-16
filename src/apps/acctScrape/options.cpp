/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("-maxBlocks:<val>", "scan at most --maxBlocks blocks ('all' implies scan to end of chain)"),
    COption("@pending",         "visit pending but not yet staged or finalized blocks"),
    COption("@useBlooms",       "use bloom filters to decide which index files to search"),
    COption("@staging",         "produce results in the staging folder instead of production folder"),
    COption("",                 "Index transactions for a given Ethereum address (or series of addresses).\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool isAll = false;
    blknum_t maxBlocks = 10000;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (arg == "--all" || arg == "-a") arg = "--maxBlocks:all";

        if (startsWith(arg, "-m:") || startsWith(arg, "--maxBlocks:")) {
            arg = substitute(substitute(arg, "-m:", ""), "--maxBlocks:", "");
            if (arg == "all") {
                isAll = true;
            } else {
                if (isUnsigned(arg))
                    maxBlocks = str_2_Uint(arg);
                else
                    return usage("Please provide an integer value of maxBlocks. Quitting...");
            }

        } else if (arg == "-p" || arg == "--pending") {
            visit |= VIS_PENDING;

        } else if (arg == "-u" || arg == "--useBlooms") {
            useBlooms = true;

        } else if (arg == "-s" || arg == "--staging") {
            fm_mode = FM_STAGING;

        } else if (startsWith(arg, "0x")) {
            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid address. Quitting...");

            // TODO(tjayrush): HARD CODED LIMIT???
            if (monitors.size() < 30) {
                CAccountWatch watch;
                watch.setValueByName("address", toLower(arg)); // don't change, sets bloom value also
                watch.setValueByName("name", toLower(arg));
                watch.extra_data = toLower("chifra/" + getVersionStr() + ": " + watch.address);
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

    // show certain fields and hide others
    manageFields(defHide, false);
    manageFields(defShow, true);
    primary = monitors[0];
    string_q configFile = "./" + primary.address + ".toml";
    if (fileExists(configFile)) {
        CToml *toml = new CToml(configFile);
        if (toml) {
            manageFields(toml->getConfigStr("fields", "hide", ""), false);
            manageFields(toml->getConfigStr("fields", "show", ""), true );
            delete toml;
        }
    }

    if (getCurlContext()->nodeRequired) {
        CBlock latest;
        getBlock(latest, "latest");

        if (!isParity() || !nodeHasTraces())
            return usage("This tool will only run if it is running against a Parity node that has tracing enabled. Quitting...");
    }

    if (isAll)
        maxBlocks = INT_MAX;

    string_q transCachePath = getMonitorPath("", fm_mode);
    if (!folderExists(transCachePath)) {
        cerr << "The cache folder '" << transCachePath << "' not found. Trying to create it." << endl;
        establishFolder(transCachePath);
        if (!folderExists(transCachePath))
            return usage("The cache folder '" + transCachePath + "' not created. Quiting...");
    }

    for (auto monitor : monitors) {
        string_q fn = getMonitorPath(monitor.address, fm_mode);
        if (fileExists(fn + ".lck"))
            return usage("The cache file '" + fn + "' is locked. Quitting...");

        fn = getMonitorLast(monitor.address, fm_mode);
        if (fileExists(fn + ".lck"))
            return usage("The last block file '" + fn + "' is locked. Quitting...");
    }

    if (!folderExists(indexFolder_finalized_v2))
        return usage("Address index path '" + indexFolder_finalized_v2 + "' not found. Quitting...");

    blknum_t lastInCache = getLastBlock_cache_final();
    startScrape = str_2_Uint(asciiFileToString(getMonitorLast(primary.address, fm_mode)));
    scrapeCnt   = min(lastInCache - startScrape, maxBlocks);

    if (verbose) {
        for (auto monitor : monitors) {
            cerr << "Freshening " << monitor.address << "\r";cerr.flush();
        }
    }
    return startScrape < lastInCache;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    // We want to be able to run this more than once
    // optionOn(OPT_RUNONCE);

    fm_mode     = FM_PRODUCTION;
    minArgs     = 0;
    startScrape = 0;
    scrapeCnt   = 0;
    visit       = (VIS_STAGING | VIS_FINAL);
    useBlooms   = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
    // just some cleanup of the screen
    cerr << string_q(150,' ') << "\r";
}
