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
    COption("mode", "", "enum[local*|remote|onchain]", OPT_REQUIRED | OPT_POSITIONAL, "the source from which to pin, unpin, or display the index hashes"),  // NOLINT
    COption("hash", "a", "", OPT_SWITCH, "display the hash instead of contents of manifest"),
    COption("pin", "p", "<string>", OPT_FLAG, "pin items either locally or remotely ('all' to all items in a folder)"),
    COption("unpin", "u", "<string>", OPT_FLAG, "unpin previously pinned items give a hash, a filename, or 'all'"),
    COption("init", "i", "", OPT_SWITCH, "initialize the TrueBlocks appearance index by downloading the bloom filters"),
    COption("license", "l", "", OPT_HIDDEN | OPT_SWITCH, "show the current pinata license information"),
    COption("", "", "", OPT_DESCRIPTION, "Report on and manage the pinned appearance index chunks and associated bloom filters."),  // NOLINT
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
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-a" || arg == "--hash") {
            hash = true;

        } else if (startsWith(arg, "-p:") || startsWith(arg, "--pin:")) {
            pin = substitute(substitute(arg, "-p:", ""), "--pin:", "");

        } else if (startsWith(arg, "-u:") || startsWith(arg, "--unpin:")) {
            unpin = substitute(substitute(arg, "-u:", ""), "--unpin:", "");

        } else if (arg == "-i" || arg == "--init") {
            init = true;

        } else if (arg == "-l" || arg == "--license") {
            license = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!mode.empty())
                return usage("Please specify only one mode.");
            if (!confirmEnum("mode", mode, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    // BEG_DEBUG_DISPLAY
    LOG_TEST("mode", mode, (mode == ""));
    LOG_TEST_BOOL("hash", hash);
    LOG_TEST("pin", pin, (pin == ""));
    LOG_TEST("unpin", unpin, (unpin == ""));
    LOG_TEST_BOOL("init", init);
    LOG_TEST_BOOL("license", license);
    // END_DEBUG_DISPLAY

    if (!pinlib_getApiKeys(lic))
        return usage("You need a pinata license to proceed.");

    if (license)
        configureDisplay("pinMan", "CPinApiLicense", STR_DISPLAY_PINAPILICENSE);
    else
        configureDisplay("pinMan", "CPinManifest", STR_DISPLAY_PINMANIFEST);

    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    // BEG_CODE_INIT
    hash = false;
    pin = "";
    unpin = "";
    init = false;
    license = false;
    // END_CODE_INIT
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
