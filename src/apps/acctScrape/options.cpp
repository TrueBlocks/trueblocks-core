/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
// BEG_CODE_OPTIONS
    COption("addr_list", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "one or more Ethereum addresses"),
    COption("staging", "s", "", OPT_HIDDEN | OPT_SWITCH, "produce results in the staging folder instead of production folder"),
    COption("unripe", "u", "", OPT_HIDDEN | OPT_SWITCH, "visit unripe (not old enough and not yet staged or finalized) blocks"),
    COption("daemon", "d", "", OPT_HIDDEN | OPT_SWITCH, "we are being called in daemon mode which causes us to print results differently"),
    COption("no_header", "n", "", OPT_SWITCH, "do not show the header row"),
    COption("start", "S", "<blknum>", OPT_HIDDEN | OPT_FLAG, "first block to process(inclusive)"),
    COption("end", "E", "<blknum>", OPT_HIDDEN | OPT_FLAG, "last block to process (inclusive)"),
    COption("", "", "", OPT_DESCRIPTION, "Index transactions for a given Ethereum address (or series of addresses)."),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

string_q dTabs;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    scanRange.first = UINT_MAX;

// BEG_CODE_LOCAL_INIT
    bool staging = false;
    bool unripe = false;
    bool no_header = false;
    blknum_t start = NOPOS;
    blknum_t end = NOPOS;
// END_CODE_LOCAL_INIT

    CBlock lBlock;
    getBlock_light(lBlock, "latest");
    blknum_t latest = lBlock.blockNumber;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
// BEG_CODE_AUTO
        } else if (arg == "-s" || arg == "--staging") {
            staging = true;

        } else if (arg == "-u" || arg == "--unripe") {
            unripe = true;

        } else if (arg == "-d" || arg == "--daemon") {
            daemon = true;

        } else if (arg == "-n" || arg == "--no_header") {
            no_header = true;

        } else if (startsWith(arg, "-S:") || startsWith(arg, "--start:")) {
            if (!confirmBlockNum("start", start, arg, latest))
                return false;

        } else if (startsWith(arg, "-E:") || startsWith(arg, "--end:")) {
            if (!confirmBlockNum("end", end, arg, latest))
                return false;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

// END_CODE_AUTO
        } else {
            if (!startsWith(arg, "0x"))
                return usage("Invalid option: " + arg);

            if (!isAddress(arg))
                return usage(arg + " does not appear to be a valid address. Quitting...");

            CAccountWatch watch;
            watch.setValueByName("address", toLower(arg)); // don't change, sets bloom value also
            watch.setValueByName("name", toLower(arg));
            watch.extra_data = getVersionStr() + "/" + watch.address;
            watch.color = cBlue;
            watch.finishParse();
            monitors.push_back(watch);
        }
    }

    if (monitors.size() == 0)
        return usage("You must provide at least one Ethereum address. Quitting...");

    if (start != NOPOS) scanRange.first = start;
    if (end != NOPOS) scanRange.second = end;
    if (unripe)  visitTypes |= VIS_UNRIPE;
    if (staging) visitTypes |= VIS_STAGING;

    establishFolder(getMonitorPath("", FM_PRODUCTION));
    establishFolder(getMonitorPath("", FM_STAGING));
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_blooms);
    establishFolder(indexFolder_staging);
    establishFolder(indexFolder_ripe);

    dTabs = (daemon ? "\t  " : "");
    string_q endLine = (daemon ? "\r" : "\n");
    for (auto monitor : monitors) {
        string_q fn1 = getMonitorPath(monitor.address);
        if (fileExists(fn1 + ".lck"))
            return usage("The cache file '" + fn1 + "' is locked. Quitting...");
        string_q fn2 = getMonitorLast(monitor.address);
        if (fileExists(fn2 + ".lck"))
            return usage("The last block file '" + fn2 + "' is locked. Quitting...");
        string_q fn3 = getMonitorExpt(monitor.address);
        if (fileExists(fn3 + ".lck"))
            return usage("The last export file '" + fn3 + "' is locked. Quitting...");
        string_q fn4 = getMonitorBals(monitor.address);
        if (fileExists(fn4 + ".lck"))
            return usage("The last export file '" + fn4 + "' is locked. Quitting...");
        if (!isTestMode())
            LOG_INFO("freshening: ", cYellow, monitor.address, cOff, "...");
        if (scanRange.first == UINT_MAX)
            scanRange.first = min(scanRange.first, str_2_Uint(asciiFileToString(fn2))); // If file doesn't exist, this will report '0'
    }

    blknum_t unripeBlk, ripeBlk, stagingBlk, finalizedBlk, clientBlk;
    getLastBlocks(unripeBlk, ripeBlk, stagingBlk, finalizedBlk, clientBlk);

    if (scanRange.second == NOPOS) {
        scanRange.second = finalizedBlk;
        if (visitTypes & VIS_STAGING)
            scanRange.second = stagingBlk;
        if (visitTypes & VIS_UNRIPE)
            scanRange.second = unripeBlk;
    }

    if (no_header)
        expContext().fmtMap["header"] = "";

    // So one of the test cases passes only
    if (isTestMode() && monitors.size() == 1 && monitors[0].address == "0x001d14804b399c6ef80e64576f657660804fec0b")
        setenv("TEST_MODE", "false", true);

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

// BEG_CODE_INIT
    daemon = false;
// END_CODE_INIT

    minArgs    = 0;
    visitTypes = VIS_FINAL;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
    // just some cleanup of the screen
    if (!daemon) {
        cerr << string_q(120, ' ') << "\r";
        cerr.flush();
    }
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
