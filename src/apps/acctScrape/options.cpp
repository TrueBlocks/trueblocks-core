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
    COption("@noBlooms",        "turn off bloom filters for performance testing"),
    COption("@staging",         "produce results in the staging folder instead of production folder"),
    COption("@start:<num>",     "first block to check"),
    COption("",                 "Index transactions for a given Ethereum address (or series of addresses).\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool isAll = false;
    blknum_t maxBlocks = 10000;
    blknum_t start     = 0;

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

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--start:")) {
            arg = substitute(substitute(arg, "-s:", ""), "--start:", "");
            if (isUnsigned(arg))
                start = str_2_Uint(arg);
            else
                return usage("Please provide an integer value for start. Quitting...");

        } else if (arg == "-p" || arg == "--pending") {
            visitTypes |= VIS_PENDING;

        } else if (arg == "-n" || arg == "--noBlooms") {
            useBlooms = false;

        } else if (startsWith(arg, "0x")) {
            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid address. Quitting...");

            // TODO(tjayrush): HARD CODED LIMIT???
            if (monitors.size() < 30) {
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
        getBlock_light(latest, "latest");

        if (!isParity() || !nodeHasTraces())
            return usage("This tool will only run if it is running against a Parity node that has tracing enabled. Quitting...");
    }

    if (isAll)
        maxBlocks = INT_MAX;

    string_q prodPath = getMonitorPath("", FM_PRODUCTION);
    if (!folderExists(prodPath)) {
        cerr << "The cache folder '" << prodPath << "' not found. Trying to create it." << endl;
        establishFolder(prodPath);
        if (!folderExists(prodPath))
            return usage("The cache folder '" + prodPath + "' not created. Quiting...");
    }

    string_q stagePath = getMonitorPath("", FM_STAGING);
    if (!folderExists(stagePath)) {
        cerr << "The cache folder '" << stagePath << "' not found. Trying to create it." << endl;
        establishFolder(stagePath);
        if (!folderExists(stagePath))
            return usage("The cache folder '" + stagePath + "' not created. Quiting...");
    }

    blknum_t lastInCache = getLastBlock_cache_final();
    for (auto monitor : monitors) {
        // Note: We've cleaned the staging folder, so we only have to check for locks in the production folder
        string_q fn = getMonitorLast(monitor.address);
        if (fileExists(fn))
            scanRange.first = min(max(scanRange.first, str_2_Uint(asciiFileToString(fn))), lastInCache);

        if (fileExists(fn + ".lck"))
            return usage("The cache file '" + fn + "' is locked. Quitting...");

        fn = getMonitorLast(monitor.address);
        if (fileExists(fn + ".lck"))
            return usage("The last block file '" + fn + "' is locked. Quitting...");

        cerr << "freshening " << monitor.address << "..." << endl;
    }
    if (start > scanRange.first && start <= lastInCache)
        scanRange.first = start;

    if (!folderExists(indexFolder_finalized))
        return usage("Address index path '" + indexFolder_finalized + "' not found. Quitting...");

    // How many should we scrape?
    ASSERT(earliestStart <= lastInCache);
    scrapeCnt = min(lastInCache - scanRange.first, maxBlocks);

    if (verbose) {
        for (auto monitor : monitors) {
            cerr << "Freshening " << monitor.address << "\r";cerr.flush();
        }
    }

    return scanRange.first < lastInCache;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    // We want to be able to run this more than once
    // optionOn(OPT_RUNONCE);
    // This app never actually writes to standard out, so we don't really need this
    // optionOn(OPT_OUTPUT);

    minArgs         = 0;
    scrapeCnt       = 0;
    visitTypes      = (VIS_STAGING | VIS_FINAL);
    useBlooms       = true;
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
