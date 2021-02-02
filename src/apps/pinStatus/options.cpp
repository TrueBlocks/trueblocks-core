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
    COption("mode", "", "enum[local*|remote|onchain]", OPT_REQUIRED | OPT_POSITIONAL, "display local or remote manifest or its IPFS hash"),  // NOLINT
    COption("hash", "a", "", OPT_SWITCH, "display the hash instead of contents of manifest ('on' for onchain mode)"),
    COption("pin", "p", "<string>", OPT_FLAG, "pin indexes and blooms, add to manifest, and return hash"),
    COption("unpin", "u", "<string>", OPT_FLAG, "unpin index(es) and blooms given a hash, a filename, or 'all'"),
    COption("license", "i", "", OPT_HIDDEN | OPT_SWITCH, "show the current pinata license information, if any"),
    COption("", "", "", OPT_DESCRIPTION, "Report on and manage pinned appearance index and bloom chunks."),
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

        } else if (arg == "-i" || arg == "--license") {
            license = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!mode.empty())
                return usage("Please specify only one mode. Quitting...");
            if (!confirmEnum("mode", mode, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    if (isRaw)
        expContext().exportFmt = JSON1;

    if (!getPinataKeys(lic))
        return usage("You need a pinata license to proceed. Quitting...");

    provider = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "http://localhost:8545");

    LOG4("pinata_api_key:\t", cGreen, lic.apiKey, cOff);
    LOG4("pinata_secret_api_key:\t", cGreen, lic.secretKey, cOff);
    if (license)
        configureDisplay("pinStatus", "CPinataLicense", STR_DISPLAY_PINATALICENSE);
    else
        configureDisplay("pinStatus", "CPinReport", STR_DISPLAY_PINREPORT);

    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    // BEG_CODE_INIT
    hash = false;
    pin = "";
    unpin = "";
    license = false;
    // END_CODE_INIT
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
    minArgs = 0;

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
