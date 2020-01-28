/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("addrs", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "one or more Ethereum addresses"),
    COption("finalized", "f", "", OPT_HIDDEN | OPT_TOGGLE, "toggle search of finalized folder ('on' by default)"),
    COption("staging", "s", "", OPT_HIDDEN | OPT_TOGGLE, "toggle search of staging (not yet finalized) folder ('off' by default)"),  // NOLINT
    COption("unripe", "u", "", OPT_HIDDEN | OPT_TOGGLE, "toggle search of unripe (neither staged nor finalized) folder ('off' by default)"),  // NOLINT
    COption("daemon", "d", "", OPT_HIDDEN | OPT_SWITCH, "we are being called in daemon mode which causes us to print results differently"),  // NOLINT
    COption("start", "S", "<blknum>", OPT_HIDDEN | OPT_FLAG, "first block to process (inclusive)"),
    COption("end", "E", "<blknum>", OPT_HIDDEN | OPT_FLAG, "last block to process (inclusive)"),
    COption("", "", "", OPT_DESCRIPTION, "Index transactions for a given Ethereum address (or series of addresses)."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    ENTER8("parseArguments");
    if (!standardOptions(command))
        EXIT_NOMSG(false);

    // BEG_CODE_LOCAL_INIT
    CAddressArray addrs;
    bool finalized = true;
    bool staging = false;
    bool unripe = false;
    blknum_t start = NOPOS;
    blknum_t end = NOPOS;
    // END_CODE_LOCAL_INIT

    // How far does the system think it is?
    LOG8("Setting up...");
    blknum_t unripeBlk, ripeBlk, stagingBlk, finalizedBlk, clientBlk;
    getLatestBlocks(unripeBlk, ripeBlk, stagingBlk, finalizedBlk, clientBlk);
    blknum_t latest = clientBlk;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-f" || arg == "--finalized") {
            finalized = !finalized;

        } else if (arg == "-s" || arg == "--staging") {
            staging = !staging;

        } else if (arg == "-u" || arg == "--unripe") {
            unripe = !unripe;

        } else if (arg == "-d" || arg == "--daemon") {
            daemon = true;

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

        } else {
            if (!parseAddressList2(this, addrs, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    for (auto addr : addrs) {
        CAccountWatch watch;
        watch.setValueByName("address", toLower(addr));  // don't change, sets bloom value also
        watch.setValueByName("name", toLower(addr));
        watch.extra_data = getVersionStr() + "/" + watch.address;
        watch.color = cBlue;
        watch.finishParse();
        monitors.push_back(watch);
    }
    LOG_INFO("Scraping ", monitors.size(), " addresses.");

    if (monitors.size() == 0)
        EXIT_USAGE("Please provide at least one Ethereum address to scrape. Quitting...");

    establishFolder(getMonitorPath("", FM_PRODUCTION));
    establishFolder(getMonitorPath("", FM_STAGING));
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_blooms);
    establishFolder(indexFolder_staging);
    establishFolder(indexFolder_ripe);

    if (unripe)
        visitTypes |= VIS_UNRIPE;
    if (staging)
        visitTypes |= VIS_STAGING;

    if (isNoHeader)
        expContext().fmtMap["header"] = "";

    // Scan the monitors to see if any are locked (fail if yes). While we're at it, find the block the monitors think we
    // should start with (that is, one more than the last block they've seen, its deploy block if we haven't seen it yet
    // and it's a smart contract, or zero).
    LOG8("Checking locks...");
    blknum_t earliestBlock = NOPOS;
    for (auto monitor : monitors) {
        if (!checkLocks(monitor.address))
            EXIT_NOMSG(false);
        earliestBlock = min(earliestBlock, nextBlockAsPerMonitor(monitor.address));
    }
    blknum_t latestBlock =
        (visitTypes & VIS_UNRIPE) ? unripeBlk : (visitTypes & VIS_STAGING) ? stagingBlk : finalizedBlk;

    scanRange = make_pair(earliestBlock, latestBlock);
    if (start != NOPOS)
        scanRange.first = start;  // the user is always right
    if (end != NOPOS)
        scanRange.second = end;  // the user is always right

    if (scanRange.first >= scanRange.second) {  // nothing to do?
        for (auto watch : monitors) {
            if (isContractAt(watch.address) && latest > scanRange.first) {
                LOG_INFO("Monitor for contract '", watch.address, "' is caught up to address indexer.");
            } else {
                LOG_INFO("Monitor for address  '", watch.address, "' is caught up to address indexer.");
            }
        }
        EXIT_NOMSG(false);
    }
    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    // BEG_CODE_INIT
    daemon = false;
    // END_CODE_INIT

    minArgs = 0;
    visitTypes = VIS_FINAL;
    monitors.clear();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("`addresses` must start with '0x' and be forty two characters long.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG
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
bool COptions::checkLocks(const address_t& address) const {
    string_q fn1 = getMonitorPath(address);
    if (fileExists(fn1 + ".lck"))
        return usage("The cache file '" + fn1 + "' is locked. Quitting...");
    string_q fn2 = getMonitorLast(address);
    if (fileExists(fn2 + ".lck"))
        return usage("The last block file '" + fn2 + "' is locked. Quitting...");
    string_q fn3 = getMonitorExpt(address);
    if (fileExists(fn3 + ".lck"))
        return usage("The last export file '" + fn3 + "' is locked. Quitting...");
    string_q fn4 = getMonitorBals(address);
    if (fileExists(fn4 + ".lck"))
        return usage("The last export file '" + fn4 + "' is locked. Quitting...");
    string_q fn5 = getMonitorCnfg(address);
    if (fileExists(fn5 + ".lck"))
        return usage("The config file '" + fn5 + "' is locked. Quitting...");
    string_q fn6 = getMonitorPath(address + ".deleted");
    if (fileExists(fn6 + ".lck"))
        return usage("The marker file '" + fn6 + "' is locked. Quitting...");
    return true;
}

//--------------------------------------------------------------------------------
blknum_t COptions::nextBlockAsPerMonitor(const address_t& address) const {
    ENTER8("nextBlockAsPerMonitor: " + address);

    if (fileExists(getMonitorLast(address)))
        return str_2_Uint(asciiFileToString(getMonitorLast(address)));

    if (getGlobalConfig("acctScrape")->getConfigBool("settings", "start-when-deployed", true)) {
        blknum_t deployed = getDeployBlock(address);
        EXIT_NOMSG8(deployed == NOPOS ? 0 : deployed);
    }

    EXIT_NOMSG8(0);
}
