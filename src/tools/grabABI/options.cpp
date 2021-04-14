/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("addrs", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "list of one or more smart contracts whose ABI to grab from EtherScan"),  // NOLINT
    COption("canonical", "c", "", OPT_SWITCH, "convert all types to their canonical represenation and remove all spaces from display"),  // NOLINT
    COption("known", "k", "", OPT_SWITCH, "load common 'known' ABIs from cache"),
    COption("sol", "s", "<string>", OPT_HIDDEN | OPT_FLAG, "file name of .sol file from which to create a new known abi (without .sol)"),  // NOLINT
    COption("find", "f", "<string>", OPT_FLAG, "try to search for a function declaration given a four byte code"),
    COption("", "", "", OPT_DESCRIPTION, "Fetches the ABI for a smart contract."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    ENTER("parseArguments: " + command);
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    bool canonical = false;
    bool known = false;
    string_q sol = "";
    string_q find = "";
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    blknum_t latest = NOPOS;  // getBlockProgress(BP_CLIENT).client;
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-c" || arg == "--canonical") {
            canonical = true;

        } else if (arg == "-k" || arg == "--known") {
            known = true;

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--sol:")) {
            sol = substitute(substitute(arg, "-s:", ""), "--sol:", "");

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--find:")) {
            find = substitute(substitute(arg, "-f:", ""), "--find:", "");

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!parseAddressList(this, addrs, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    // BEG_DEBUG_DISPLAY
    LOG_TEST_LIST("addrs", addrs, addrs.empty());
    LOG_TEST_BOOL("canonical", canonical);
    LOG_TEST_BOOL("known", known);
    LOG_TEST("sol", sol, (sol == ""));
    LOG_TEST("find", find, (find == ""));
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;

    if (!find.empty()) {
        ostringstream os;
        os << "findSig " << find;
        LOG_TEST_CALL(os.str());
        // clang-format off
        if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on
        return false;
    }

    if (!sol.empty()) {
        if (!fileExists(sol + ".sol") && !fileExists(sol))
            return usage("Cannot find .sol file at '" + sol + "'.");
        convertFromSol(substitute(sol, ".sol", ""));
        cerr << sol << " coverted in current folder." << endl;
        return false;
    }

    if (isRaw) {
        for (auto addr : addrs) {
            string_q fileName = getCachePath("abis/" + addr + ".json");
            string_q localFile("./" + addr + ".json");
            if (fileExists(localFile)) {
                LOG_INFO("Using local ABI file");
                fileName = localFile;
            }
            if (!fileExists(fileName))
                return usage("ABI for '" + addr + "' not found.");
            cout << asciiFileToString(fileName) << endl;
        }
        return false;
    }

    if (!addrs.size() && !known)
        return usage("Please supply at least one Ethereum address.\n");

    if (canonical)
        parts |= SIG_CANONICAL;

    if (parts == 0)
        parts = SIG_DEFAULT;

    if (parts != SIG_CANONICAL && verbose)
        parts |= SIG_DETAILS;

    if (known)
        abi_spec.loadAbisFromKnown();

    for (auto addr : addrs) {
        bool testing = isTestMode() && addr == "0xeeeeeeeeddddddddeeeeeeeeddddddddeeeeeeee";
        string_q fileName = getCachePath("abis/" + addr + ".json");
        if (!testing && !isContractAt(addr, latest) && !fileExists(fileName)) {
            cerr << "Address " << addr << " is not a smart contract. Skipping..." << endl;
        } else {
            abi_spec.loadAbiFromEtherscan(addr, isRaw);
            abi_spec.address = addr;
        }
    }

    // Display formatting
    string_q format = STR_DISPLAY_ABI;
    string_q ffields = toLower(substitute(substitute(substitute(STR_DISPLAY_FUNCTION, "[{", ""), "}]", ""), "\t", ","));
    string_q funcFields = "CFunction:" + ffields;
    if (isTestMode())
        funcFields = "CFunction:" + substitute(ffields, "inputs,outputs", "input_names,output_names");

    replace(format, "[{ADDRESS}]\t", "");
    replace(funcFields, "address,", "");

    if (verbose && (expContext().exportFmt == JSON1 || expContext().exportFmt == API1)) {
        replaceAll(funcFields, "_name", "");
        replaceAll(format, "_NAME", "");
    }

    configureDisplay("grabABI", "CAbi", format);
    manageFields("CFunction:all", false);
    manageFields(funcFields, true);
    manageFields("CParameter:all", false);
    manageFields("CParameter:type,name,internalType,components,is_array,indexed", true);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RAW);

    // BEG_CODE_INIT
    // END_CODE_INIT

    parts = (SIG_DEFAULT | SIG_ENCODE);
    addrs.clear();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    first = true;
    Init();
    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("Solidity files found in the local folder with the name '<address>.sol' are converted to an ABI prior to processing (and then removed).");  // NOLINT
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//-----------------------------------------------------------------------
void COptions::convertFromSol(const address_t& addr) {
    abi_spec.loadAbiFromSolidity(addr);
    GETRUNTIME_CLASS(CFunction)->sortFieldList();
    GETRUNTIME_CLASS(CParameter)->sortFieldList();
    if (isTestMode()) {
        HIDE_FIELD(CParameter, "is_array");
        HIDE_FIELD(CParameter, "is_builtin");
        HIDE_FIELD(CParameter, "is_minimal");
        HIDE_FIELD(CParameter, "is_object");
        HIDE_FIELD(CParameter, "is_pointer");
        HIDE_FIELD(CParameter, "no_write");
        HIDE_FIELD(CParameter, "str_default");
        HIDE_FIELD(CParameter, "value");
    }
    expContext().spcs = 2;

    ostringstream os;
    os << abi_spec;

    string_q fileName = addr + ".json";
    ::remove(fileName.c_str());
    stringToAsciiFile(fileName, os.str());

    isRaw = false;
}
