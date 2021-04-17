/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#define LOGGING_LEVEL_TEST
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("list", "l", "enum[local*|remote]", OPT_FLAG, "list all pins either locally or at the pinning service"),
    COption("compare", "c", "", OPT_SWITCH, "report differences (if any) between the manifest and pinning service"),
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
    LOG_TEST_BOOL("compare", compare);
    LOG_TEST_BOOL("init", init);
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;

    LOG_INFO("hashToIndexFormatFile:\t", cGreen, hashToIndexFormatFile, cOff);
    LOG_INFO("hashToBloomFormatFile:\t", cGreen, hashToBloomFormatFile, cOff);
    LOG_INFO("unchainedIndexAddrV3:\t", cGreen, unchainedIndexAddrV3, cOff);
    LOG_INFO("unchainedIndexAddr:\t", cGreen, unchainedIndexAddr, cOff);
    LOG_INFO("manifestHashEncoding:\t", cGreen, manifestHashEncoding, cOff);

    if (!list.empty() + compare + init > 1)
        return usage("Please choose only a single option.");

    if (!list.empty() + compare + init == 0)
        return usage("You must specify at least one option.");

    configureDisplay("pinMan", "CPinnedChunk", STR_DISPLAY_PINNEDCHUNK);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    // BEG_CODE_INIT
    list = "";
    compare = false;
    init = false;
    // END_CODE_INIT
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    firstOut = true;

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
            stringToAsciiFile(configPath("manifest/manifest.txt"), contents);
            localPins.clear();
        }
    } else {
        LOG_INFO("Manifest is up to data at: ", currManifest);
    }

    if (download)
        pinlib_forEveryPin(localPins, checkOnDisc, NULL);

    return true;
}

/*
 if (publish && pin)
 return usage("The --publish option is not available when using the --pin option.");
 LOG_TEST_BOOL("publish", publish);
 } else if (arg == "-u" || arg == "--publish") {
 publish = true;
 COption("publish", "u", "", OPT_SWITCH, "publish the hash of the pin manifest to the UnchainedIndex smart contract"),
// NOLINT
 // Is the user asking to publish the pin manifest to the smart contract?
 if (publish) {
 CPinManifest manifest;
 manifest.fileName = "manifest.txt";
 manifest.indexFormat = hashToIndexFormatFile;
 manifest.bloomFormat = hashToBloomFormatFile;

 CPinnedChunkArray pList;
 pinlib_r eadPinList(pList, true);
 pinlib_forEveryPin(pList, addNewPin, &manifest);
 manifest.toJson(cout);

 return true;
 }
 //----------------------------------------------------------------
bool addNewPin(CPinnedChunk& newPin, void* data) {
    CPinManifest* manifestPtr = (CPinManifest*)data;  // NOLINT
    manifestPtr->pins.push_back(newPin);

    timestamp_t unused;
    blknum_t newEnd;
    blknum_t newStart = bnFromPath(newPin.fileName, newEnd, unused);

    manifestPtr->firstPin = min(manifestPtr->firstPin, newStart);
    manifestPtr->lastPin = min(manifestPtr->lastPin, newEnd);
    return !isTestMode();
}
 */
