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
    COption("list", "l", "enum[local*|remote]", OPT_FLAG, "list all pins either locally or at the pinning service"),
    COption("freshen", "f", "", OPT_SWITCH, "push files from the local index to the pinning service and update manifest"),  // NOLINT
    COption("compare", "c", "", OPT_SWITCH, "report differences between the manifest and the pinning service"),
    COption("init", "i", "", OPT_SWITCH, "initialize the local index by downloading bloom filters from the pinning service"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Report on and manage the remotely pinned appearance index and associated bloom filters."),  // NOLINT
    // clang-format on
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
        } else if (startsWith(arg, "-l:") || startsWith(arg, "--list:")) {
            if (!confirmEnum("list", list, arg))
                return false;
        } else if (arg == "-l" || arg == "--list") {
            return usage("The --list option requires a value.");

        } else if (arg == "-f" || arg == "--freshen") {
            freshen = true;

        } else if (arg == "-c" || arg == "--compare") {
            compare = true;

        } else if (arg == "-i" || arg == "--init") {
            init = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

            // END_CODE_AUTO
        }
    }

    // BEG_DEBUG_DISPLAY
    LOG_TEST("list", list, (list == ""));
    LOG_TEST_BOOL("freshen", freshen);
    LOG_TEST_BOOL("compare", compare);
    LOG_TEST_BOOL("init", init);
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;

    if (freshen + compare + init > 1)
        return usage("Please choose only a single option.");

    configureDisplay("pinMan", "CPinManifest", STR_DISPLAY_PINMANIFEST);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    // BEG_CODE_INIT
    list = "";
    freshen = false;
    compare = false;
    init = false;
    // END_CODE_INIT
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
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

//----------------------------------------------------------------
hash_t COptions::getCurrentManifest(void) {
    CEthCall theCall;
    theCall.address = unchainedIndexAddr;
    theCall.encoding = manifestHashEncoding;
    theCall.blockNumber = getBlockProgress(BP_CLIENT).client;
    loadAbiFile(configPath("abis/known-000/unchained.json"), &theCall.abi_spec);
    return doEthCall(theCall) ? theCall.result.outputs[0].value : "";
}

//----------------------------------------------------------------
hash_t getLastManifest(void) {
    return asciiFileToString(configPath("ipfs-hashes/lastHash.txt"));
}

//----------------------------------------------------------------
bool checkOnDisc(CPinnedChunk& pin, void* data) {
    pin.onDisc = fileExists(pin.fileName);
    return true;
}

//-------------------------------------------------------------------------
string_q getFileContentsByHash(const hash_t& hash) {  // also unzips if the file is zipped
    string_q cmd = "curl -s ";
    cmd += "\"https://ipfs.io/ipfs/" + hash + "\" ";
    return doCommand(cmd);
}

//----------------------------------------------------------------
bool COptions::freshenBlooms(bool download, const string_q& currManifest) {
    string_q prev = getLastManifest();
    if (currManifest != prev) {
        LOG_INFO("Manifest needs to be updated. Previous [", prev, "] Current [", currManifest, "]");
        stringToAsciiFile(configPath("ipfs-hashes/lastHash.txt"), currManifest);
        string_q contents = getFileContentsByHash(currManifest);
        if (contents != "empty file") {
            stringToAsciiFile(configPath("manifest/initial-manifest.json"), contents);
            pList.clear();
        }
    } else {
        LOG_INFO("Manifest is up to data at: ", currManifest);
    }

    pinlib_readPinList(pList, false);
    if (download)
        pinlib_forEveryPin(pList, checkOnDisc, NULL);

    return true;
}
