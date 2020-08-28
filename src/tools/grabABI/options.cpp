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
    COption("generate", "g", "", OPT_SWITCH, "generate C++ code into the current folder for all functions and events found in the ABI"),  // NOLINT
    COption("monitored", "m", "", OPT_SWITCH, "load ABIs from monitored addresses"),
    COption("known", "k", "", OPT_SWITCH, "load common 'known' ABIs from cache"),
    COption("addr", "a", "", OPT_SWITCH, "include address of smart contract for the abi in output"),
    COption("sol", "s", "<string>", OPT_HIDDEN | OPT_FLAG, "file name of .sol file from which to create a new known abi (without .sol)"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Fetches the ABI for a smart contract. Optionally generates C++ source code representing that ABI."),  // NOLINT
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern bool sortByFuncName(const CFunction& f1, const CFunction& f2);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    bool canonical = false;
    bool monitored = false;
    bool known = false;
    bool addr = false;
    string_q sol = "";
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    blknum_t latest = NOPOS;  // getLatestBlock_client();
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-c" || arg == "--canonical") {
            canonical = true;

        } else if (arg == "-g" || arg == "--generate") {
            generate = true;

        } else if (arg == "-m" || arg == "--monitored") {
            monitored = true;

        } else if (arg == "-k" || arg == "--known") {
            known = true;

        } else if (arg == "-a" || arg == "--addr") {
            addr = true;

        } else if (startsWith(arg, "-s:") || startsWith(arg, "--sol:")) {
            sol = substitute(substitute(arg, "-s:", ""), "--sol:", "");

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!parseAddressList2(this, addrs, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    if (!sol.empty()) {
        if (!fileExists(sol + ".sol") && !fileExists(sol))
            return usage("Cannot find .sol file at '" + sol + "'. Quitting...");
        convertFromSol(substitute(sol, ".sol", ""));
        cerr << sol << " coverted in current folder." << endl;
        return false;
    }

    if (isRaw) {
        for (auto a : addrs) {
            string_q fileName = getCachePath("abis/" + a + ".json");
            string_q localFile("./" + a + ".json");
            if (fileExists(localFile)) {
                LOG_INFO("Using local ABI file");
                fileName = localFile;
            }
            if (!fileExists(fileName))
                return usage("ABI for '" + a + "' not found. Quitting...");
            cout << asciiFileToString(fileName) << endl;
        }
        return false;
    }

    if (!addrs.size() && !known && !monitored)
        return usage("Please supply at least one Ethereum address.\n");

    if (generate) {
        classDir = getCWD();
        prefix = getPrefix(classDir);
    }

    if (canonical)
        parts |= SIG_CANONICAL;

    if (parts == 0)
        parts = SIG_DEFAULT;

    if (parts != SIG_CANONICAL && verbose)
        parts |= SIG_DETAILS;

    for (auto a : addrs) {
        bool testing = isTestMode() && a == "0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
        if (!testing && !isContractAt(a, latest)) {
            cerr << "Address " << a << " is not a smart contract. Skipping..." << endl;
        } else {
            CAbi abi;
            loadAbiAndCache(abi, a, isRaw, errors);
            if (errors.size() > 0) {
                ostringstream os;
                for (auto err : errors)
                    os << err;
                // report but do not quit processing
                cerr << os.str() << endl;
            }
            abi.address = a;
            for (auto i = abi.interfaces.begin(); i != abi.interfaces.end(); i++)
                i->address = a;
            sort(abi.interfaces.begin(), abi.interfaces.end(), sortByFuncName);
            abiList.push_back(abi);
        }
    }

    if (known) {
        CAbi abi;
        abi.loadAbiKnown();
        abiList.push_back(abi);
    }

    if (monitored) {
        CAbi abi;
        abi.loadAbisMonitors();
        sort(abi.interfaces.begin(), abi.interfaces.end(), sortByFuncName);
        abiList.push_back(abi);
    }

    if (generate)
        handle_generate();

    // Display formatting
    string_q format = STR_DISPLAY_ABI;
    string_q ffields = toLower(substitute(substitute(substitute(STR_DISPLAY_FUNCTION, "[{", ""), "}]", ""), "\t", ","));
    string_q funcFields = "CFunction:" + ffields;
    if (isTestMode())
        funcFields = "CFunction:" + substitute(ffields, "inputs,outputs", "input_names,output_names");

    if (!addr) {
        replace(format, "[{ADDRESS}]\t", "");
        replace(funcFields, "address,", "");
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

    removeDuplicateEncodings(abiList);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RAW | OPT_OUTPUT);

    // BEG_CODE_INIT
    generate = false;
    // END_CODE_INIT

    parts = (SIG_DEFAULT | SIG_ENCODE);
    addrs.clear();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
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

//--------------------------------------------------------------------------------
string_q getPrefix(const string_q& inIn) {
    string_q in = inIn;                   // for example ./ENS/parselib/
    replace(in, "parseLib", "parselib");  // hack: to fix dao monitor
    reverse(in);
    replace(in, "/", "");          // remove trailing '/'
    in = nextTokenClear(in, '/');  // remove /parselib
    reverse(in);
    return in;
}

//---------------------------------------------------------------------------
bool visitABIs(const string_q& path, void* dataPtr) {
    if (endsWith(path, ".json")) {
        string_q* str = reinterpret_cast<string_q*>(dataPtr);
        *str += (path + "\n");
    }
    return true;
}

//-----------------------------------------------------------------------
bool sortByFuncName(const CFunction& f1, const CFunction& f2) {
    string_q s1 = (f1.type == "event" ? "zzzevent" : f1.type) + f1.name + f1.encoding;
    for (auto f : f1.inputs)
        s1 += f.name;
    string_q s2 = (f2.type == "event" ? "zzzevent" : f2.type) + f2.name + f2.encoding;
    for (auto f : f2.inputs)
        s2 += f.name;
    return s1 < s2;
}

//-----------------------------------------------------------------------
void COptions::convertFromSol(const address_t& a) {
    CAbi abi;
    sol_2_Abi(abi, a);
    bool first1 = true;
    expContext().spcs = 2;
    ostringstream os;
    os << "[" << endl;
    incIndent();
    for (auto func : abi.interfaces) {
        if (!first1)
            os << ",";
        os << endl;
        os << "\n    " << func;
        first1 = false;
    }
    decIndent();
    os << endl << "]" << endl;
    ::remove((a + ".json").c_str());
    stringToAsciiFile(a + ".json", os.str());
    isRaw = false;
}
