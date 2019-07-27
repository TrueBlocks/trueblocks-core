/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    COption("-nBlocks:<num>",     "maximum number of blocks to process (defaults to 5000)"),
    COption("@nBlockProcs:<num>", "number of block channels for blaze"),
    COption("@nAddrProcs:<num>",  "number of address channels for blaze"),
    COption("",                   "Decentralized blockchain scraper and block cache.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

#define indexFolder_sorted (getIndexPath("sorted/"))
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (startsWith(arg, "-n:") || startsWith(arg, "--nBlocks:")) {
            arg = substitute(substitute(arg, "-n:", ""), "--nBlocks:", "");
            if (!isUnsigned(arg))
                return usage("--nBlocks must be a non-negative number. Quitting...");
            nBlocks = str_2_Uint(arg);

        } else if (startsWith(arg, "--nBlockProcs:")) {
            arg = substitute(arg, "--nBlockProcs:", "");
            if (!isUnsigned(arg))
                return usage("--nBlockProcs must be a non-negative number. Quitting...");
            nBlockProcs = str_2_Uint(arg);

        } else if (startsWith(arg, "--nAddrProcs:")) {
            arg = substitute(arg, "--nAddrProcs:", "");
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
    establishFolder(indexFolder_sorted);
    establishFolder(indexFolder_finalized);
    establishFolder(indexFolder_blooms);
    establishFolder(indexFolder_staging);
    establishFolder(indexFolder_unripe);
    establishFolder(indexFolder_ripe);
    establishFolder(configPath("cache/tmp/"));

    CBlock latest;
    getBlock_light(latest, "latest");
    latestBlockTs = latest.timestamp;
    latestBlockNum = latest.blockNumber;

    if (!isParity() || !nodeHasTraces()) {
        string_q errMsg = "You must run Parity with --tracing on for this tool to work.";
        if (getEnvStr("DOCKER_MODE") == "true")
            errMsg += " Under docker, enable remote rpc (see Parity help).";
        return usage(errMsg);
    }

    string_q zeroBin = indexFolder_finalized + padNum9(0) + "-" + padNum9(0) + ".bin";
    if (!fileExists(zeroBin)) {
        ASSERT(prefunds.size() == 8893);  // This is a known value
        cerr << "\tBuilding origin block index" << endl;
        CStringArray appearances;
        for (auto prefund : prefunds) {
            CStringArray parts;
            explode(parts, prefund, '\t');
            // The prefund transactions have a zero for thier block numbers and an index
            // into thier location in the list of presale addresses. We need to do this so we
            // can distringuish them when they are exported.
            ostringstream os;
            os << parts[0] << "\t" << padNum9(0) << "\t" << padNum5((uint32_t)appearances.size()) << endl;
            appearances.push_back(os.str());
        }
        writeIndexAsBinary(zeroBin, appearances); // also writes the bloom file
    }

    const CToml *config = getGlobalConfig("blockScrape");
    nBlocks     = config->getConfigInt("settings", "nBlocks",     (nBlocks     == NOPOS ? 2000 : nBlocks    ));
    nBlockProcs = config->getConfigInt("settings", "nBlockProcs", (nBlockProcs == NOPOS ?   10 : nBlockProcs));
    nAddrProcs  = config->getConfigInt("settings", "nAddrProcs",  (nAddrProcs  == NOPOS ?   20 : nAddrProcs ));

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RUNONCE | OPT_PREFUND);

    nBlockProcs = NOPOS;
    nAddrProcs  = NOPOS;
    nBlocks     = NOPOS;
    minArgs     = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
