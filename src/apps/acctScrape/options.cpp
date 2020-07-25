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
    COption("staging", "s", "", OPT_HIDDEN | OPT_SWITCH, "enable search of staging (not yet finalized) folder"),
    COption("unripe", "u", "", OPT_HIDDEN | OPT_SWITCH, "enable search of unripe (neither staged nor finalized) folder (requires --staging)"),  // NOLINT
    COption("blooms", "b", "", OPT_HIDDEN | OPT_SWITCH, "process query by first using bloom filter and, if hit, downloading index chunk from remote"),  // NOLINT
    COption("start", "S", "<blknum>", OPT_HIDDEN | OPT_FLAG, "this value is ignored but remains for backward compatibility"),  // NOLINT
    COption("end", "E", "<blknum>", OPT_HIDDEN | OPT_FLAG, "this value is ignored but remains for backward compatibility"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Index transactions for a given Ethereum address (or collection of addresses)."),  // NOLINT
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    ENTER("parseArguments");
    if (!standardOptions(command))
        EXIT_NOMSG(false);

    // BEG_CODE_LOCAL_INIT
    CAddressArray addrs;
    bool staging = false;
    bool unripe = false;
    blknum_t start = NOPOS;
    blknum_t end = NOPOS;
    // END_CODE_LOCAL_INIT

    // How far does the system think it is?
    blknum_t unripeBlk, ripeBlk, stagingBlk, finalizedBlk, latest;
    getLatestBlocks(unripeBlk, ripeBlk, stagingBlk, finalizedBlk, latest);

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

        } else if (arg == "-b" || arg == "--blooms") {
            blooms = true;

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

    // Make sure we have the folders we need (may be redundant, but harmless)...
    establishMonitorFolders();
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_blooms);
    establishFolder(indexFolder_staging);
    establishFolder(indexFolder_ripe);

    // Are we visiting unripe and/or staging in our search?
    if (staging)
        visitTypes |= VIS_STAGING;
    if (unripe) {
        if (!(visitTypes & VIS_STAGING))
            EXIT_USAGE("You must also specify --staging when using --unripe. Quitting...");
        visitTypes |= VIS_UNRIPE;
    }

    // Where will we start?
    blknum_t nextBlockToVisit = NOPOS;

    // We need at least one address to scrape...
    if (addrs.size() == 0)
        EXIT_USAGE("You must provide at least one Ethereum address. Quitting...");

    // Accumulate the addresses into the monitors list and decide where we should start
    for (auto addr : addrs) {
        CMonitor monitor;
        monitor.setValueByName("address", addr);  // do not remove, this also sets the bloom value for the address
        monitor.finishParse();
        string_q msg;
        if (monitor.isLocked(msg))  // If locked, we fail
            EXIT_USAGE(msg);
        nextBlockToVisit = min(nextBlockToVisit, monitor.nextBlockAsPerMonitor());
        monitors.push_back(monitor);
    }

    // Last block depends on scrape type or user input --end (with appropriate check)
    blknum_t lastBlockToVisit =
        (visitTypes & VIS_UNRIPE) ? unripeBlk : (visitTypes & VIS_STAGING) ? stagingBlk : finalizedBlk;

    // Mark the range...
    scanRange = make_pair(nextBlockToVisit, lastBlockToVisit);
    if (isTestMode()) {
        scanRange.first = max(scanRange.first, start);
        scanRange.second = max(scanRange.second, end);
    }

    // If the chain is behind the monitor (for example, the user is re-syncing), quit silently...
    if (latest < scanRange.first) {
        LOG4("Chain is behind the monitor.");
        EXIT_NOMSG(false);
    }

    // If there's nothing to scrape, quit silently...
    if (scanRange.first >= scanRange.second) {
        LOG4("Account scraper is up to date.");
        EXIT_NOMSG(false);
    }

    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    // BEG_CODE_INIT
    blooms = false;
    // END_CODE_INIT

    minArgs = 0;
    fileRange = make_pair(NOPOS, NOPOS);
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
}
