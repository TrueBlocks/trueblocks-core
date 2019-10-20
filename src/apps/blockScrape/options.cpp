/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
// BEG_CODE_OPTIONS
    COption("n_blocks", "n", "<blknum>", OPT_FLAG, "maximum number of blocks to process (defaults to 5000)"),
    COption("n_block_procs", "p", "<uint>", OPT_HIDDEN | OPT_FLAG, "number of block channels for blaze"),
    COption("n_addr_procs", "a", "<uint>", OPT_HIDDEN | OPT_FLAG, "number of address channels for blaze"),
    COption("", "", "", OPT_DESCRIPTION, "Decentralized blockchain scraper and block cache."),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

// BEG_CODE_LOCAL_INIT
// END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
// BEG_CODE_AUTO
// END_CODE_AUTO

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--n_blocks:")) {
            arg = substitute(substitute(arg, "-n:", ""), "--n_blocks:", "");
            if (!isUnsigned(arg))
                return usage("--n_blocks must be a non-negative number. Quitting...");
            n_blocks = str_2_Uint(arg);

        } else if (startsWith(arg, "-p:") || startsWith(arg, "--n_block_procs:")) {
            arg = substitute(substitute(arg, "--n_block_procs:", ""), "-p:", "");
            if (!isUnsigned(arg))
                return usage("--n_block_procs must be a non-negative number. Quitting...");
            n_block_procs = str_2_Uint(arg);

        } else if (startsWith(arg, "-a:") || startsWith(arg, "--n_addr_procs:")) {
            arg = substitute(substitute(arg, "--n_addr_procs:", ""), "-a:", "");
            if (!isUnsigned(arg))
                return usage("--n_addr_procs must be a non-negative number. Quitting...");
            n_addr_procs = str_2_Uint(arg);

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
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
const char* STR_ERROR_MSG =
"{\n"
"  \"message\": \"Reporting for test mode only\",\n"
"  \"errors\": [\n"
"    \"n_blocks [N_BLOCKS]\",\n"
"    \"n_block_procs [N_BLOCK_PROCS]\",\n"
"    \"n_addr_procs [N_ADDR_PROCS]\"\n"
"  ]\n"
"}\n";
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

    CBlock latest;
    getBlock_light(latest, "latest");
    latestBlockTs = latest.timestamp;
    latestBlockNum = latest.blockNumber;

    string_q zeroBin = indexFolder_finalized + padNum9(0) + "-" + padNum9(0) + ".bin";
    if (!fileExists(zeroBin)) {
        ASSERT(prefunds.size() == 8893);  // This is a known value
        cerr << "\tBuilding origin block index" << endl;
        CStringArray appearances;
        for (auto prefund : prefundWeiMap) {
            // The prefund transactions have a zero for thier block numbers and an index
            // into thier location in the list of presale addresses. We need to do this so we
            // can distringuish them when they are exported.
            ostringstream os;
            os << prefund.first << "\t" << padNum9(0) << "\t" << padNum5((uint32_t)appearances.size()) << endl;
            appearances.push_back(os.str());
        }
        writeIndexAsBinary(zeroBin, appearances); // also writes the bloom file
    }

    const CToml *config = getGlobalConfig("blockScrape");
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

    n_blocks      = config->getConfigInt("settings", "n_blocks",      (n_blocks      == NOPOS ? 2000 : n_blocks     ));
    n_block_procs = config->getConfigInt("settings", "n_block_procs", (n_block_procs == NOPOS ?   10 : n_block_procs));
    n_addr_procs  = config->getConfigInt("settings", "n_addr_procs",  (n_addr_procs  == NOPOS ?   20 : n_addr_procs ));

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RUNONCE | OPT_PREFUND);

// BEG_CODE_INIT
// END_CODE_INIT

    if (getEnvStr("DOCKER_MODE") == "true") {
        n_blocks      = 100;
        n_block_procs = 5;
        n_addr_procs  = 10;
    } else {
        n_blocks      = NOPOS;
        n_block_procs = NOPOS;
        n_addr_procs  = NOPOS;
    }

    minArgs     = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
