/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
// BEG_CODE_OPTIONS
    COption("nBlocks", "n", "<blknum>", OPT_FLAG, "maximum number of blocks to process (defaults to 5000)"),
    COption("nBlockProcs", "b", "<uint>", OPT_HIDDEN | OPT_FLAG, "number of block channels for blaze"),
    COption("nAddrProcs", "a", "<uint>", OPT_HIDDEN | OPT_FLAG, "number of address channels for blaze"),
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

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--nBlocks:")) {
            arg = substitute(substitute(arg, "-n:", ""), "--nBlocks:", "");
            if (!isUnsigned(arg))
                return usage("--nBlocks must be a non-negative number. Quitting...");
            nBlocks = str_2_Uint(arg);

        } else if (startsWith(arg, "-b:") || startsWith(arg, "--nBlockProcs:")) {
            arg = substitute(substitute(arg, "--nBlockProcs:", ""), "-b:", "");
            if (!isUnsigned(arg))
                return usage("--nBlockProcs must be a non-negative number. Quitting...");
            nBlockProcs = str_2_Uint(arg);

        } else if (startsWith(arg, "-a:") || startsWith(arg, "--nAddrProcs:")) {
            arg = substitute(substitute(arg, "--nAddrProcs:", ""), "-a:", "");
            if (!isUnsigned(arg))
                return usage("--nAddrProcs must be a non-negative number. Quitting...");
            nAddrProcs = str_2_Uint(arg);

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
"    \"nBlocks [NBLOCKS]\",\n"
"    \"nBlockProcs [NBLOCKPROCS]\",\n"
"    \"nAddrProcs [NADDRPROCS]\"\n"
"  ]\n"
"}\n";
            string_q msg = STR_ERROR_MSG;
            replace(msg, "[NBLOCKS]", uint_2_Str(nBlocks));
            replace(msg, "[NBLOCKPROCS]", uint_2_Str(nBlockProcs));
            replace(msg, "[NADDRPROCS]", uint_2_Str(nAddrProcs));
            cout << msg << endl;
        } else {
            cout << "Reporting for test mode only:" << endl;
            cout << "\tnBlocks: " << nBlocks << endl;
            cout << "\tnBlockProcs: " << nBlockProcs << endl;
            cout << "\tnAddrProcs: " << nAddrProcs << endl;
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

    nBlocks     = config->getConfigInt("settings", "nBlocks",     (nBlocks     == NOPOS ? 2000 : nBlocks    ));
    nBlockProcs = config->getConfigInt("settings", "nBlockProcs", (nBlockProcs == NOPOS ?   10 : nBlockProcs));
    nAddrProcs  = config->getConfigInt("settings", "nAddrProcs",  (nAddrProcs  == NOPOS ?   20 : nAddrProcs ));

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RUNONCE | OPT_PREFUND);

// BEG_CODE_INIT
// END_CODE_INIT

    if (getEnvStr("DOCKER_MODE") == "true") {
        nBlocks     = 100;
        nBlockProcs = 5;
        nAddrProcs  = 10;
    } else {
        nBlocks     = NOPOS;
        nBlockProcs = NOPOS;
        nAddrProcs  = NOPOS;
    }

    minArgs     = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
