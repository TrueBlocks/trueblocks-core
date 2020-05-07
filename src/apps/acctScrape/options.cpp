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
    COption("start", "S", "<blknum>", OPT_HIDDEN | OPT_FLAG, "first block to process (inclusive)"),
    COption("end", "E", "<blknum>", OPT_HIDDEN | OPT_FLAG, "last block to process (inclusive)"),
    COption("silent", "i", "", OPT_HIDDEN | OPT_SWITCH, "lighten the reporting on progress (for use with --daemon switch to `chifra scrape`)"),  // NOLINT
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
    bool finalized = true;
    bool staging = false;
    bool unripe = false;
    blknum_t start = NOPOS;
    blknum_t end = NOPOS;
    bool silent = false;
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
        } else if (arg == "-f" || arg == "--finalized") {
            finalized = !finalized;

        } else if (arg == "-s" || arg == "--staging") {
            staging = !staging;

        } else if (arg == "-u" || arg == "--unripe") {
            unripe = !unripe;

        } else if (startsWith(arg, "-S:") || startsWith(arg, "--start:")) {
            if (!confirmBlockNum("start", start, arg, latest))
                return false;

        } else if (startsWith(arg, "-E:") || startsWith(arg, "--end:")) {
            if (!confirmBlockNum("end", end, arg, latest))
                return false;

        } else if (arg == "-i" || arg == "--silent") {
            silent = true;

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

#ifdef MOCK_DATA
    if (mockData) {
        cerr << "Mocking" << endl;
        verbose = 10;
        for (size_t i = 0; i < 10000; i++) {
            blknum_t s = (i * 10000) + (i * 13);
            blknum_t e = ((i + 1) * 10000) - 1 + (i * 13);
            bool swit = (i % 3);
            ostringstream os;
            os << (swit ? "Skip" : "Scan") << " mocks ./" << padNum9(s) << "-" << padNum9(e) << "\r";
            LOG_INFO(os.str());
            usleep(250);
        }
        cout << "{\"address\": \"0xf503017d7baf7fbc0fff7492b751025c6a78179b\",\"blockNumber\": "
                "9931310,\"transactionIndex\": 21 }"
             << endl;
        return false;
    }
#endif

    // We need at least one address to scrape...
    if (addrs.size() == 0)
        EXIT_USAGE("You must provide at least one Ethereum address. Quitting...");

    // Make sure we have the folders we need (may be redundant, but harmless)...
    establishFolder(getMonitorPath("", FM_PRODUCTION));
    establishFolder(getMonitorPath("", FM_STAGING));
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_blooms);
    establishFolder(indexFolder_staging);
    establishFolder(indexFolder_ripe);

    // Are we visiting unripe and/or staging in our search?
    if (unripe)
        visitTypes |= VIS_UNRIPE;
    if (staging)
        visitTypes |= VIS_STAGING;

    // Clean up a bit and accumulate the addresses into the monitors list...
    for (auto addr : addrs) {
        CAccountWatch monitor;
        // do not remove the next line, it also sets the bloom value for this address
        monitor.setValueByName("address", addr);
        monitor.finishParse();
        monitors.push_back(monitor);
    }
    if (!silent)
        LOG_INFO("Scraping ", monitors.size(), " addresses.");

    //    if (isNoHeader)
    //        expContext().fmtMap["header"] = "";

    // Scan the monitors to see if any are locked (fail if so)...
    for (auto monitor : monitors)
        if (!checkLocks(monitor.address))
            EXIT_NOMSG(false);

    //
    // Find the earliest block we need to start scanning at
    //   For each monitor...
    //     if we've not seen this address before...
    //       next_block = is_contract ? deploy_block : 0
    //     else
    //       next_block = last visited block + 1
    blknum_t nextBlockToVisit = NOPOS;
    for (auto monitor : monitors)
        nextBlockToVisit = min(nextBlockToVisit, nextBlockAsPerMonitor(monitor.address));

    blknum_t lastBlockToVisit =
        (visitTypes & VIS_UNRIPE) ? unripeBlk : (visitTypes & VIS_STAGING) ? stagingBlk : finalizedBlk;

    // Where we would start by ourselves
    scanRange = make_pair(nextBlockToVisit, lastBlockToVisit);

    // TODO(tjayrush): Should this be protected from bad user input?
    // Has the user told us where to start and stop?
    scanRange.first = (start == NOPOS ? scanRange.first : start);
    scanRange.second = (end == NOPOS ? scanRange.second : end);

    if ((contains(command, "--start") || contains(command, "--end")) && start != NOPOS)
        if (scanRange.first >= scanRange.second)
            EXIT_USAGE("'start' option must be strictly less than 'end' option. Quitting...");

    if (scanRange.first >= scanRange.second && latest > scanRange.first) {  // nothing to do?
        if (!silent) {
            for (auto monitor : monitors) {
                ostringstream os;
                string_q acctType = (isContractAt(monitor.address, latest) ? "contract" : "address");
                os << "Monitor for " << acctType << " '" << monitor.address << "' is caught up to indexer.";
                LOG_INFO(os.str());
            }
        }
        EXIT_NOMSG(false);
    }

    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_MOCKDATA);

    // BEG_CODE_INIT
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

//--------------------------------------------------------------------------------
#define checkLock(a, b)                                                                                                \
    {                                                                                                                  \
        string_q fn = (a);                                                                                             \
        if (fileExists(fn + ".lck"))                                                                                   \
            return usage("The " + string_q(b) + " file '" + fn + "' is locked. Quitting...");                          \
    }

bool COptions::checkLocks(const address_t& address) const {
    checkLock(getMonitorPath(address), "cache");
    checkLock(getMonitorLast(address), "last block");
    checkLock(getMonitorExpt(address), "last export");
    checkLock(getMonitorBals(address), "last export");
    checkLock(getMonitorCnfg(address), "config");
    checkLock(getMonitorPath(address) + ".deleted", "marker");
    return true;
}

//--------------------------------------------------------------------------------
blknum_t COptions::nextBlockAsPerMonitor(const address_t& address) const {
    if (fileExists(getMonitorLast(address)))
        return str_2_Uint(asciiFileToString(getMonitorLast(address)));

    // Contracts can receive ether prior to being deployed (counter-factually). By default, we ignore
    // this and start our scan at the deploy block. EOAs return NOPOS as deploy block.
    if (getGlobalConfig("acctScrape")->getConfigBool("settings", "start-when-deployed", true)) {
        blknum_t deployed = getDeployBlock(address);
        return (deployed == NOPOS ? 0 : deployed);
    }
    return 0;
}
