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
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("addrs", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "a list of one or more smart contracts whose ABIs to display"),  // NOLINT
    COption("known", "k", "", OPT_SWITCH, "load common 'known' ABIs from cache"),
    COption("sol", "s", "list<string>", OPT_FLAG, "extract the abi definition from the provided .sol file(s)"),
    COption("find", "f", "list<string>", OPT_FLAG, "search for function or event declarations given a four- or 32-byte code(s)"),  // NOLINT
    COption("source", "o", "", OPT_HIDDEN | OPT_SWITCH, "show the source of the ABI information"),
    COption("classes", "c", "", OPT_HIDDEN | OPT_SWITCH, "generate classDefinitions folder and class definitions"),
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
    bool known = false;
    CStringArray sol;
    CStringArray find;
    bool source = false;
    bool classes = false;
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    blknum_t latest = NOPOS;  // getBlockProgress(BP_CLIENT).client;
    string_q unused;
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
        } else if (isKnownAbi(arg, unused)) {
            addrs.push_back(arg);

            // BEG_CODE_AUTO
        } else if (arg == "-k" || arg == "--known") {
            known = true;

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--sol:")) {
            arg = substitute(substitute(arg, "-s:", ""), "--sol:", "");
            sol.push_back(arg);
        } else if (arg == "-s" || arg == "--sol") {
            return flag_required("sol");

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--find:")) {
            arg = substitute(substitute(arg, "-f:", ""), "--find:", "");
            find.push_back(arg);
        } else if (arg == "-f" || arg == "--find") {
            return flag_required("find");

        } else if (arg == "-o" || arg == "--source") {
            source = true;

        } else if (arg == "-c" || arg == "--classes") {
            classes = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            if (!parseAddressList(this, addrs, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    // BEG_DEBUG_DISPLAY
    LOG_TEST_LIST("addrs", addrs, addrs.empty());
    LOG_TEST_BOOL("known", known);
    LOG_TEST_LIST("sol", sol, sol.empty());
    LOG_TEST_LIST("find", find, find.empty());
    LOG_TEST_BOOL("source", source);
    LOG_TEST_BOOL("classes", classes);
    // END_DEBUG_DISPLAY

    if (Mocked(""))
        return false;

    if (!find.empty()) {
        for (auto f : find) {
            ostringstream os;
            os << getCommandPath("findSig") << " " << f;
            LOG_TEST_CALL(os.str());
            // clang-format off
            if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
            // clang-format on
        }
        return false;
    }

    if (!sol.empty()) {
        for (auto s : sol) {
            if (!fileExists(s + ".sol") && !fileExists(s))
                return usage(substitute(usageErrs[ERR_CANNOTFINDSOL], "[{SOL}]", s));
            convertFromSol(substitute(s, ".sol", ""));
            cerr << s << " coverted in current folder." << endl;
        }
        return false;
    }

    if (!addrs.size() && !known)
        return usage(usageErrs[ERR_SUPPLYONEADDR]);

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
            abi_spec.loadAbiFromEtherscan(addr);
            abi_spec.address = addr;
        }
    }

    if (classes) {
        return usage(usageErrs[ERR_OPTIONNOTIMPL]);
#if 0
        for (auto item : abi_spec.interfaceMap) {
            CFunction func = item.second;
            establishFolder("./classes/classDefinitions/");
            ostringstream os;
            os << "[settings]" << endl;
            os << "base_class = CTransaction" << endl;
            os << "class = C" << toUpper(string_q(1, func.name[0])) << func.name.substr(1,1000) << endl;
            os << "fields =";
            bool first = true;
            for (auto field : func.inputs) {
                if (!first) os << " |\\" << endl;
                os << "  " << field.type << " " << field.name;
                first = false;
            }
            cerr << "Writing to ./classes/classDefinitions/" << toLower(func.name) << ".txt" << endl;
            cerr << substitute(os.str(), "\t", "  ");
            stringToAsciiFile("./classes/classDefinitions/" + toLower(func.name) + ".txt", os.str());
        }
        return false;
#endif
    }

    // Display formatting
    string_q format = STR_DISPLAY_ABI;
    string_q ffields = toLower(substitute(substitute(substitute(STR_DISPLAY_FUNCTION, "[{", ""), "}]", ""), "\t", ","));
    string_q funcFields = "CFunction:" + ffields;
    if (isTestMode())
        funcFields = "CFunction:" + substitute(ffields, "inputs,outputs", "input_names,output_names");
    replace(format, "[{ADDRESS}]\t", "");
    replace(funcFields, "address,", "");
    if (!source) {
        replace(format, "[{ABI_SOURCE}]\t", "");
        replace(funcFields, "abi_source,", "");
    }

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
    registerOptions(nParams, params, OPT_RAW, OPT_DENOM);

    // BEG_CODE_INIT
    // END_CODE_INIT

    parts = (SIG_DEFAULT | SIG_ENCODE);
    addrs.clear();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("For the --sol option, place the solidity files in the current working folder.");
    notes.push_back("Search for either four byte signatures or event signatures with the --find option.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    usageErrs[ERR_CANNOTFINDSOL] = "Cannot find .sol file at '[{SOL}]'.";
    usageErrs[ERR_SUPPLYONEADDR] = "Please supply at least one Ethereum address.";
    usageErrs[ERR_OPTIONNOTIMPL] = "--classes option is not implemented.";
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//-----------------------------------------------------------------------
void COptions::convertFromSol(const address_t& addr) {
    abi_spec.loadAbiFromSolidity(addr);
    GETRUNTIME_CLASS(CFunction)->sortFieldList();
    GETRUNTIME_CLASS(CParameter)->sortFieldList();
    // TODO: This is terrible. Can we remove it?
    if (isTestMode()) {
        HIDE_FIELD(CParameter, "value");
        HIDE_FIELD(CParameter, "str_default");
        HIDE_FIELD(CParameter, "is_array");
        HIDE_FIELD(CParameter, "is_builtin");
        HIDE_FIELD(CParameter, "is_object");
        HIDE_FIELD(CParameter, "is_pointer");
        HIDE_FIELD(CParameter, "is_minimal");
        HIDE_FIELD(CParameter, "is_noaddfld");
        HIDE_FIELD(CParameter, "is_nowrite");
        HIDE_FIELD(CParameter, "is_omitempty");
        HIDE_FIELD(CParameter, "is_extra");
    }
    expContext().spcs = 2;

    ostringstream os;
    os << abi_spec;

    string_q fileName = addr + ".json";
    ::remove(fileName.c_str());
    stringToAsciiFile(fileName, os.str());
}
