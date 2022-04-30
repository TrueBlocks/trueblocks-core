/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#define LOGGING_LEVEL_TEST
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("pin", "p", "", OPT_SWITCH, "pin chunks (and blooms) to IPFS as they are created (requires pinning service)"),  // NOLINT
    COption("publish", "u", "", OPT_SWITCH, "after pinning the chunk, publish it to UnchainedIndex"),
    COption("block_cnt", "n", "<uint64>", OPT_FLAG, "maximum number of blocks to process per pass"),
    COption("block_chan_cnt", "b", "<uint64>", OPT_FLAG, "number of concurrent block processing channels"),
    COption("addr_chan_cnt", "d", "<uint64>", OPT_FLAG, "number of concurrent address processing channels"),
    COption("reset", "e", "<blknum>", OPT_HIDDEN | OPT_FLAG, "reset the scraper to the provided block (or end of earliest chunk prior to that block)"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Scan the chain and update (and optionally pin) the TrueBlocks index of appearances."),  // NOLINT
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern bool visitPrefund(const Allocation& prefund, void* data);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    Init();

    // BEG_CODE_LOCAL_INIT
    blknum_t reset = NOPOS;
    // END_CODE_LOCAL_INIT

    blknum_t latest = getLatestBlock_client();
    CBlock block;
    getBlockHeader(block, latest);

    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-p" || arg == "--pin") {
            pin = true;

        } else if (arg == "-u" || arg == "--publish") {
            publish = true;

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--block_cnt:")) {
            if (!confirmUint("block_cnt", block_cnt, arg))
                return false;
        } else if (arg == "-n" || arg == "--block_cnt") {
            return flag_required("block_cnt");

        } else if (startsWith(arg, "-b:") || startsWith(arg, "--block_chan_cnt:")) {
            if (!confirmUint("block_chan_cnt", block_chan_cnt, arg))
                return false;
        } else if (arg == "-b" || arg == "--block_chan_cnt") {
            return flag_required("block_chan_cnt");

        } else if (startsWith(arg, "-d:") || startsWith(arg, "--addr_chan_cnt:")) {
            if (!confirmUint("addr_chan_cnt", addr_chan_cnt, arg))
                return false;
        } else if (arg == "-d" || arg == "--addr_chan_cnt") {
            return flag_required("addr_chan_cnt");

        } else if (startsWith(arg, "-e:") || startsWith(arg, "--reset:")) {
            if (!confirmBlockNum("reset", reset, arg, latest))
                return false;
        } else if (arg == "-e" || arg == "--reset") {
            return flag_required("reset");

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

            // END_CODE_AUTO
        }
    }

    if (reset != NOPOS) {
        return handle_reset();
    }

    if (pin && !getApiKey(lic)) {
        if (!isTestMode()) {
            ostringstream os;
            os << "The --pin option requires you to have a Pinata key.";
            return usage(os.str());
        }
    }

    // We can't really test this code, so we just report and quit
    if (isTestMode()) {
        ostringstream os;
        os << "{" << endl;
        os << "  \"message\": \"Testing in blockScrape/options.cpp\"," << endl;
        os << "  \"block_cnt\": " << block_cnt << "," << endl;
        os << "  \"block_chan_cnt\": " << block_chan_cnt << "," << endl;
        os << "  \"addr_chan_cnt\": " << addr_chan_cnt << "," << endl;
        os << "  \"pin\": " << pin << "," << endl;
        os << "  \"publish\": " << publish << "," << endl;
        os << "}" << endl;
        cout << os.str();
        return false;
    }

    // Debugging data...
    LOG_DIR8(indexFolder);
    LOG_DIR8(indexFolder_finalized);
    LOG_DIR8(indexFolder_blooms);
    LOG_DIR8(indexFolder_staging);
    LOG_DIR8(indexFolder_unripe);
    LOG_DIR8(indexFolder_ripe);

    // We need this below...
    const CToml* config = getGlobalConfig("blockScrape");

    // A tracing node is required. Otherwise, the index will be 'short' (i.e. it will
    // be missing appearances). This is okay, if the user tells us it's okay. For
    // example, if the user only wants an index of event emitters or something
    bool needsTracing = config->getConfigBool("requires", "tracing", true);
    if (needsTracing && !isTracingNode()) {
        string_q errMsg = "Tracing is required for this program to work properly.";
        if (isDockerMode())
            errMsg += " If you're running docker, enable remote RPC endpoints.";
        return usage(errMsg);
    }

    if (!isArchiveNode())
        return usage("This tool requires historical balances which your RPC server does not provide.");

    // This may be the first time we've ever run. In that case, we need to build the zero block index file...
    string chunkId = padNum9(0) + "-" + padNum9(0);
    string_q bloomZeroPath = indexFolder_blooms + chunkId + ".bloom";
    if (!fileExists(bloomZeroPath)) {
        if (!loadPrefundBalances())
            return usage("Could not load prefunds database.");

        // Each chain must have it's own prefund addresses. Here, we scan the prefund list
        // and add a psuedo-transaction (block: 0, txid: order-in-file) for each address.
        // Tradition has it that the prefund list is sorted by address.
        CStringArray consolidatedLines;
        forEveryPrefund(visitPrefund, &consolidatedLines);

        // Write the chunk and the bloom to the binary cache
        string_q chunkPath = indexFolder_finalized + chunkId + ".bin";
        if (!writeIndexAsBinary(chunkPath, consolidatedLines, (pin ? visitToPin : nullptr), &pinList)) {
            LOG_ERR(cRed, "Failed to write index chunk for block zero.", cOff);
            return false;
        }
        ostringstream os;
        os << "Wrote " << consolidatedLines.size() << " records to " << cTeal << relativize(chunkPath) << cOff;
        LOG_INFO(os.str());
    }

    // The previous run may have quit early, leaving the folders in a mild state of
    // disrepair. We clean that up here.
    ::remove((indexFolder_staging + "000000000-temp.txt").c_str());
    cleanFolder(indexFolder_unripe);

    return !isRunning("acctExport");
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, OPT_PREFUND, OPT_FMT);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    pin = false;
    publish = false;
    // clang-format off
    block_cnt = getGlobalConfig("blockScrape")->getConfigInt("settings", "block_cnt", 2000);
    block_chan_cnt = getGlobalConfig("blockScrape")->getConfigInt("settings", "block_chan_cnt", 10);
    addr_chan_cnt = getGlobalConfig("blockScrape")->getConfigInt("settings", "addr_chan_cnt", 20);
    apps_per_chunk = getGlobalConfig("blockScrape")->getConfigInt("settings", "apps_per_chunk", 200000);
    unripe_dist = getGlobalConfig("blockScrape")->getConfigInt("settings", "unripe_dist", 28);
    snap_to_grid = getGlobalConfig("blockScrape")->getConfigInt("settings", "snap_to_grid", 100000);
    first_snap = getGlobalConfig("blockScrape")->getConfigInt("settings", "first_snap", 0);
    allow_missing = getGlobalConfig("blockScrape")->getConfigBool("settings", "allow_missing", false);
    // clang-format on
    // END_CODE_INIT

    meta = getMetaData();

    if (getChain() == "mainnet") {
        // different defaults for mainnet
        apps_per_chunk = apps_per_chunk == 200000 ? 2000000 : apps_per_chunk;
        first_snap = first_snap == 0 ? 2250000 : first_snap;
    }

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    // clang-format on
    // END_CODE_NOTES

    // clang-format off
    configs.push_back("`block_cnt`: maximum number of blocks to process (defaults to 5000).");
    configs.push_back("`block_chan_cnt`: number of concurrent block channels for blaze.");
    configs.push_back("`addr_chan_cnt`: number of concurrent address channels for blaze.");
    // clang-format on

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS

    // Establish the folders that hold the data...
    establishCacheFolders();
    establishIndexFolders();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//-----------------------------------------------------------------------
bool visitPrefund(const Allocation& prefund, void* data) {
    ostringstream os;

    CStringArray* appearances = (CStringArray*)data;
    os << prefund.address << "\t" << padNum9(0) << "\t" << padNum5((uint32_t)appearances->size()) << endl;
    appearances->push_back(os.str());

    return true;
}
