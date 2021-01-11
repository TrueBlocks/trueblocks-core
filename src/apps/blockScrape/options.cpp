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
    COption("mode", "", "list<enum[run*|quit|pause|restart]>", OPT_POSITIONAL, "control the block and account scrapers"),  // NOLINT
    COption("tool", "t", "list<enum[monitors|index*|none|both]>", OPT_FLAG, "process the index, monitors, or both (none means process timestamps only)"),  // NOLINT
    COption("n_blocks", "n", "<blknum>", OPT_FLAG, "maximum number of blocks to process (defaults to 5000)"),
    COption("n_block_procs", "b", "<uint64>", OPT_HIDDEN | OPT_FLAG, "number of block channels for blaze"),
    COption("n_addr_procs", "a", "<uint64>", OPT_HIDDEN | OPT_FLAG, "number of address channels for blaze"),
    COption("pin", "p", "", OPT_SWITCH, "pin new chunks (and blooms) to IPFS (requires Pinata key)"),
    COption("publish", "u", "", OPT_SWITCH, "publish the hash of the pin manifest to the UnchainedIndex smart contract"),  // NOLINT
    COption("listpins", "l", "", OPT_HIDDEN | OPT_SWITCH, "list pins (precludes other options, requires API key)"),
    COption("", "", "", OPT_DESCRIPTION, "Decentralized blockchain scraper and block cache."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    ENTER("parseArguments");

    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    bool listpins = false;
    // END_CODE_LOCAL_INIT

    blknum_t latest = getLatestBlock_client();
    CBlock block;
    getBlock_light(block, latest);
    latestBlockTs = block.timestamp;
    latestBlockNum = block.blockNumber;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (startsWith(arg, "-t:") || startsWith(arg, "--tool:")) {
            string_q tool_tmp;
            if (!confirmEnum("tool", tool_tmp, arg))
                return false;
            tool.push_back(tool_tmp);

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--n_blocks:")) {
            if (!confirmBlockNum("n_blocks", n_blocks, arg, latest))
                return false;

        } else if (startsWith(arg, "-b:") || startsWith(arg, "--n_block_procs:")) {
            if (!confirmUint("n_block_procs", n_block_procs, arg))
                return false;

        } else if (startsWith(arg, "-a:") || startsWith(arg, "--n_addr_procs:")) {
            if (!confirmUint("n_addr_procs", n_addr_procs, arg))
                return false;

        } else if (arg == "-p" || arg == "--pin") {
            pin = true;

        } else if (arg == "-u" || arg == "--publish") {
            publish = true;

        } else if (arg == "-l" || arg == "--listpins") {
            listpins = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            string_q mode_tmp;
            if (!confirmEnum("mode", mode_tmp, arg))
                return false;
            mode.push_back(mode_tmp);

            // END_CODE_AUTO
        }
    }

    // Establish the folders that hold the data...
    establishMonitorFolders();
    establishFolder(indexFolder);
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_blooms);
    establishFolder(indexFolder_staging);
    establishFolder(indexFolder_unripe);
    establishFolder(indexFolder_ripe);
    establishFolder(configPath("cache/tmp/"));

    if (mode.size() > 1)
        return usage("You must specify only one of run, quit, pause, or restart. Quitting...");
    if (mode.size() == 0)
        mode.push_back("run");

    if (tool.size() > 1)
        return usage("You must specify only one of none, index, monitors, both. Quitting...");
    if (tool.empty())
        tool.push_back("index");
    if (tool[0] == "none")
        tools = TOOL_NONE;
    else if (tool[0] == "index")
        tools = TOOL_INDEX;
    else if (tool[0] == "monitors")
        tools = TOOL_MONITORS;
    else
        tools = TOOL_BOTH;

    bool hasPinCmd = (listpins || pin || publish);
    if (hasPinCmd) {
        if (!isTestMode() && !hasPinataKeys()) {
            return usage(
                "In order to use the pin options, you must enter a Pinata key in ~/.quickBlocks/blockScrape.toml. "
                "Quitting...");

        } else if (listpins) {
            return usage("The 'listpin' option is not yet implemented. Quitting...");

        } else if (publish && !pin) {
            return usage("The --publish option is only available with the --pin option. Quitting...");
        }
    }

    if (isTestMode()) {
        cout << "{" << endl;
        cout << "  \"message\": \"Testing only\""
             << "," << endl;
        cout << "  \"mode\": \"" << mode[0] << "\"," << endl;
        cout << "  \"tool\": \"" << tool[0] << "\"," << endl;
        cout << "  \"tools\": " << tools << "," << endl;
        cout << "  \"n_blocks\": " << n_blocks << "," << endl;
        cout << "  \"n_block_procs\": " << n_block_procs << "," << endl;
        cout << "  \"n_addr_procs\": " << n_addr_procs << "," << endl;
        cout << "  \"pin\": " << pin << "," << endl;
        cout << "  \"publish\": " << publish << "," << endl;
        cout << "  \"listpins\": " << listpins << ", " << endl;
        cout << "}" << endl;
        return false;
    }

    LOG4("indexPath: ", indexFolder);
    LOG4("finalized: ", indexFolder_finalized);
    LOG4("blooms: ", indexFolder_blooms);
    LOG4("staging: ", indexFolder_staging);
    LOG4("unripe: ", indexFolder_unripe);
    LOG4("ripe: ", indexFolder_ripe);
    LOG4("tmp: ", configPath("cache/tmp/"));

    string_q zeroBloom = getIndexPath("blooms/" + padNum9(0) + "-" + padNum9(0) + ".bloom");
    if (!fileExists(zeroBloom)) {
        ASSERT(prefundWeiMap.size() == 8893);  // This is a known value
        LOG_INFO("Index for block zero was not found. Building it from ", uint_2_Str(prefundWeiMap.size()),
                 " prefunds.");

        CStringArray appearances;
        for (auto prefund : prefundWeiMap) {
            // The prefund transactions have 'zero' block numbers and an index into thier location
            // in the list of presale addresses which is sorted by address. We need to do this in order to
            // distinquish each transaction when it is exported.
            ostringstream os;
            os << prefund.first << "\t" << padNum9(0) << "\t" << padNum5((uint32_t)appearances.size()) << endl;
            appearances.push_back(os.str());
        }

        string_q zeroBin = getIndexPath("finalized/" + padNum9(0) + "-" + padNum9(0) + ".bin");
        writeIndexAsBinary(zeroBin, appearances);  // also writes the bloom file
        if (pin) {
            CPinnedItem pinRecord;
            pinChunk(padNum9(0) + "-" + padNum9(0), pinRecord);
            if (publish)
                publishManifest(cout);
        }
        LOG_INFO("Done...");
    }

    const CToml* config = getGlobalConfig("blockScrape");
    bool needsParity = config->getConfigBool("requires", "parity", true);
    if (needsParity && !isParity())
        return usage(
            "This tool requires Parity. Add [requires]\\nparity=false to ~/.quickBlocks/blockScrape.toml turn this "
            "test off. Quitting...");

    bool needsTracing = config->getConfigBool("requires", "tracing", true);
    if (needsTracing && !nodeHasTraces()) {
        string_q errMsg = "You must be running Parity with `--tracing on` for this tool to work properly.";
        if (isDockerMode())
            errMsg += " If you're running docker, enable remote RPC endpoints (see Parity help).";
        return usage(errMsg);
    }

    bool needsBalances = config->getConfigBool("requires", "balances", false);
    if (needsBalances && !nodeHasBalances(true))
        return usage("This tool requires an --archive node with historical balances. Quitting...");

    if (isAlreadyRunning(g_progName)) {
        LOG_WARN("The " + getProgName() + " is already running. Quitting...");
        return false;
    }

    // Do not run if the index is being searched...
    if (isRunning("acctScrape")) {
        LOG_WARN("Refusing to run while acctScrape is running. Will restart shortly...");
        return false;
    }

    // The previous run may have quit early, leaving the folders in a mild state of disrepair. Clean up first.
    ::remove((indexFolder_staging + "000000000-temp.txt").c_str());
    cleanFolder(indexFolder_unripe);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND);
    optionOff(OPT_FMT);
    // Since we need prefunds, let's load the names library here
    CAccountName unused;
    getNamedAccount(unused, "0x0");

    // BEG_CODE_INIT
    tool.clear();
    n_blocks = NOPOS;
    n_block_procs = NOPOS;
    n_addr_procs = NOPOS;
    pin = false;
    publish = false;
    // END_CODE_INIT

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
    // BEG_CODE_NOTES
    // clang-format off
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool isAlreadyRunning(const string_q& progName) {
    string_q pList = listProcesses(progName);
    cout << "pList: " << pList << endl << string_q(80, '-') << endl;
    replaceAll(pList, "`", "");  // remove separators if present
    cout << "pList: " << pList << endl << string_q(80, '-') << endl;
    replaceAll(pList, progName, "`");  // change program name to separator
    cout << "pList: " << pList << endl << string_q(80, '-') << endl;
    replace(pList, "`", "");  // remove ourselves
    cout << "pList: " << pList << endl << string_q(80, '-') << endl;
    size_t count = countOf(pList, '`');
    cout << "count: " << count << endl << string_q(80, '-') << endl << endl;
    return count > 0;
}
