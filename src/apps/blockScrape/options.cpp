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
    COption("n_blocks", "n", "<blknum>", OPT_FLAG, "maximum number of blocks to process (defaults to 5000)"),
    COption("n_block_procs", "p", "<uint64>", OPT_HIDDEN | OPT_FLAG, "number of block channels for blaze"),
    COption("n_addr_procs", "a", "<uint64>", OPT_HIDDEN | OPT_FLAG, "number of address channels for blaze"),
    COption("", "", "", OPT_DESCRIPTION, "Decentralized blockchain scraper and block cache."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_ERROR_MSG;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    // END_CODE_LOCAL_INIT

    Init();
    blknum_t latest = getLatestBlock_client();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (startsWith(arg, "-n:") || startsWith(arg, "--n_blocks:")) {
            if (!confirmBlockNum("n_blocks", n_blocks, arg, latest))
                return false;

        } else if (startsWith(arg, "-p:") || startsWith(arg, "--n_block_procs:")) {
            if (!confirmUint("n_block_procs", n_block_procs, arg))
                return false;

        } else if (startsWith(arg, "-a:") || startsWith(arg, "--n_addr_procs:")) {
            if (!confirmUint("n_addr_procs", n_addr_procs, arg))
                return false;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

            // END_CODE_AUTO

        } else {
            return usage("Invalid option: " + arg);
        }
    }

    // Establish the folders that hold the data...
    // Do not collapse -- getIndexPath does not create it if it doesn't exist
    string_q indexPath = getGlobalConfig()->getConfigStr("settings", "indexPath", "<not-set>");
    if (indexPath != "<not-set>")
        establishFolder(indexPath);
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_blooms);
    establishFolder(indexFolder_staging);
    establishFolder(indexFolder_unripe);
    establishFolder(indexFolder_ripe);
    establishFolder(configPath("cache/tmp/"));

    if (isTestMode()) {
        if (isApiMode()) {
            string_q msg = STR_ERROR_MSG;
            replace(msg, "[N_BLOCKS]", uint_2_Str(n_blocks));
            replace(msg, "[N_BLOCK_PROCS]", uint_2_Str(n_block_procs));
            replace(msg, "[N_ADDR_PROCS]", uint_2_Str(n_addr_procs));
            cout << msg << endl;
        } else {
            cout << "Reporting for test mode only:" << endl;
            cout << "\tn_blocks: " << n_blocks << endl;
            cout << "\tn_block_procs: " << n_block_procs << endl;
            cout << "\tn_addr_procs: " << n_addr_procs << endl;
        }
        return false;
    }

    LOG4("indexPath: " + indexPath);
    LOG4("finalized: " + indexFolder_finalized);
    LOG4("blooms: " + indexFolder_blooms);
    LOG4("staging: " + indexFolder_staging);
    LOG4("unripe: " + indexFolder_unripe);
    LOG4("ripe: " + indexFolder_ripe);
    LOG4("tmp: " + configPath("cache/tmp/"));

    CBlock latestBlock;
    getBlock_light(latestBlock, "latest");
    latestBlockTs = latestBlock.timestamp;
    latestBlockNum = latestBlock.blockNumber;

    string_q zeroBin = getIndexPath("finalized/" + padNum9(0) + "-" + padNum9(0) + ".bin");
    if (!fileExists(zeroBin)) {
        LOG_INFO("Origin block index (" + zeroBin + ") not found. Building it from " +
                 uint_2_Str(prefundWeiMap.size()) + " prefunds.");
        ASSERT(prefundWeiMap.size() == 8893);  // This is a known value
        CStringArray appearances;
        for (auto prefund : prefundWeiMap) {
            // The prefund transactions have a zero for thier block numbers and an index
            // into thier location in the list of presale addresses. We need to do this so we
            // can distringuish them when they are exported.
            ostringstream os;
            os << prefund.first << "\t" << padNum9(0) << "\t" << padNum5((uint32_t)appearances.size()) << endl;
            appearances.push_back(os.str());
        }
        LOG_INFO("Writing index...");
        writeIndexAsBinary(zeroBin, appearances);  // also writes the bloom file
        LOG_INFO("Done...");
    }

    const CToml* config = getGlobalConfig("blockScrape");
    bool needsParity = config->getConfigBool("requires", "parity", true);
    if (needsParity && !isParity())
        return usage("This tool requires Parity. Quitting...");

    bool needsTracing = config->getConfigBool("requires", "tracing", true);
    if (needsTracing && !nodeHasTraces()) {
        string_q errMsg = "You must be running Parity with `--tracing on` for this tool to work properly.";
        if (getEnvStr("DOCKER_MODE") == "true")
            errMsg += " If you're running docker, enable remote RPC endpoints (see Parity help).";
        return usage(errMsg);
    }

    bool needsBalances = config->getConfigBool("requires", "balances", false);
    if (needsBalances && !nodeHasBalances(true))
        return usage("This tool requires an --archive node with historical balances. Quitting...");

    n_blocks = config->getConfigInt("settings", "n_blocks", (n_blocks == NOPOS ? 2000 : n_blocks));
    n_block_procs = config->getConfigInt("settings", "n_block_procs", (n_block_procs == NOPOS ? 10 : n_block_procs));
    n_addr_procs = config->getConfigInt("settings", "n_addr_procs", (n_addr_procs == NOPOS ? 20 : n_addr_procs));

#ifdef MAC
// TODO(tjayrush): fix this on non-mac machines
    if (nRunning("blockScrape")) {
        LOG_WARN("The " + getProgName() + " process may only run once. Quitting...");
        return false;
    }
#endif

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_PREFUND);
    optionOff(OPT_FMT);

    // BEG_CODE_INIT
    n_blocks = NOPOS;
    n_block_procs = NOPOS;
    n_addr_procs = NOPOS;
    // END_CODE_INIT
    if (getEnvStr("DOCKER_MODE") == "true") {
        n_blocks = 100;
        n_block_procs = 5;
        n_addr_procs = 10;
    }

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
const char* STR_ERROR_MSG =
    "{\n"
    "  \"message\": \"Reporting for test mode only\",\n"
    "  \"errors\": [\n"
    "    \"n_blocks [N_BLOCKS]\",\n"
    "    \"n_block_procs [N_BLOCK_PROCS]\",\n"
    "    \"n_addr_procs [N_ADDR_PROCS]\"\n"
    "  ]\n"
    "}\n";
