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
#include <algorithm>
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    COption("addrs", "", "list<addr>", OPT_REQUIRED | OPT_POSITIONAL, "list of one or more smart contracts whose ABI to grab from EtherScan"),
    COption("canonical", "c", "", OPT_SWITCH, "convert all types to their canonical represenation and remove all spaces from display"),
    COption("generate", "g", "", OPT_SWITCH, "generate C++ code into the current folder for all functions and events found in the ABI"),
    COption("noconst", "n", "", OPT_SWITCH, "generate encodings for non-constant functions and events only (always true when generating)"),
    COption("sol", "s", "<path>", OPT_FLAG, "create the ABI file from a .sol file in the local directory"),
    COption("known", "k", "", OPT_HIDDEN | OPT_SWITCH, "load common 'known' ABIs from cache"),
    COption("", "", "", OPT_DESCRIPTION, "Fetches the ABI for a smart contract. Optionally generates C++ source code representing that ABI."),
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern bool sortByFuncName(const CFunction& f1, const CFunction& f2);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    ENTER8("parseArguments");
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    bool canonical = false;
    bool known = false;
    // END_CODE_LOCAL_INIT

    Init();
    bool from_sol = false;
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-c" || arg == "--canonical") {
            canonical = true;

        } else if (arg == "-g" || arg == "--generate") {
            generate = true;

        } else if (arg == "-n" || arg == "--noconst") {
            noconst = true;

        } else if (arg == "-k" || arg == "--known") {
            known = true;

            // END_CODE_AUTO
        } else if (startsWith(arg, "-l:") || startsWith(arg, "--sol:")) {
            string_q orig = arg;
            arg = substitute(substitute(arg, "-s:", ""), "--sol:", "");
            if (!fileExists(arg))
                arg += ".sol";
            if (!fileExists(arg))
                EXIT_USAGE("Solidity file " + orig + " not found in local folder.");
            from_sol = true;
            addrs.push_back(substitute(arg, ".sol", ""));

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!isAddress(arg))
                return usage("Invalid address '" + arg + "'. Length (" + uint_2_Str(arg.length()) + ") is not equal to 40 characters (20 bytes).\n");
            if (!isTestMode() && !isContractAt(arg)) {
                cerr << "Address " << arg << " is not a smart contract. Skipping...";

            } else {
                addrs.push_back(toLower(str_2_Addr(arg)));
            }
        }
    }

    if (!addrs.size() && !known)
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

    if (from_sol) {
        for (auto addr : addrs) {
            CAbi abi;
            if (!sol_2_Abi(abi, addr))
                return usage("Could not find solidity file '" + addr + ".sol' in order to convert to ABI. Quitting...");
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

            ::remove((addr + ".json").c_str());
            stringToAsciiFile(addr + ".json", os.str());
            isRaw = false;
        }
    }

    if (isRaw) {
        for (auto addr : addrs) {
            string_q fileName = getCachePath("abis/" + addr + ".json");
            string_q localFile("./" + addr + ".json");
            if (fileExists(localFile)) {
                cerr << "Local file found\n";
                fileName = localFile;
            }
            if (!fileExists(fileName))
                return usage("ABI for '" + addr + "' not found. Quitting...");
            string_q contents;
            asciiFileToString(fileName, contents);
            cout << contents << "\n";
        }
        return false;
    }

    for (auto addr : addrs) {
        CAbi abi;
        loadAbiAndCache(abi, addr, isRaw, errors);
        if (errors.size() > 0) {
            ostringstream os;
            for (auto err : errors)
                os << err;
            return usage(os.str());
        }
        abi.address = addr;
        sort(abi.interfaces.begin(), abi.interfaces.end(), sortByFuncName);
        abis.push_back(abi);
    }

    if (known) {
        string_q knownPath = configPath("known_abis/");
        CStringArray files;
        listFilesInFolder(files, knownPath + "*.*", false);
        for (auto file : files) {
            CAbi abi;
            abi.loadAbiFromFile(file, true);
            sort(abi.interfaces.begin(), abi.interfaces.end(), sortByFuncName);
            abi.address = substitute(substitute(file, ".json",""), configPath("known_abis/"), "");
            abis.push_back(abi);
        }
    }

    if (generate)
        handle_generate();

    // Display formatting
    string_q format = "";  // \t[{TYPE}] [{NAME}]\t[{DECLARATION}]\t+[{ENCODING}]+";
    switch (exportFmt) {
    case NONE1:
    case TXT1:
    case CSV1:
        format = getGlobalConfig("grabABI")->getConfigStr("display", "format", format.empty() ? STR_DISPLAY_ABI : format);
        manageFields("CAbi:" + cleanFmt(format, exportFmt));
        break;
    case API1:
    case JSON1:
        format = "";
        break;
    }
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format, exportFmt);
    if (isNoHeader)
        expContext().fmtMap["header"] = "";

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(OPT_RAW | OPT_OUTPUT);

    // BEG_CODE_INIT
    generate = false;
    noconst = false;
    // END_CODE_INIT

    parts = (SIG_DEFAULT|SIG_ENCODE);
    addrs.clear();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    first = true;
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options")
        return substitute(str, "addrs", "<address> [address...]");
    return str;
}

//--------------------------------------------------------------------------------
string_q getPrefix(const string_q& inIn) {

    string_q in = inIn;  // for example ./ENS/parselib/
    replace(in, "parseLib", "parselib");  // hack: to fix dao monitor
    reverse(in);
    replace(in, "/", "");  // remove trailing '/'
    in = nextTokenClear(in, '/');  // remove /parselib
    reverse(in);
    return in;
}

//---------------------------------------------------------------------------
bool visitABIs(const string_q& path, void *dataPtr) {

    if (endsWith(path, ".json")) {
        string_q *str = reinterpret_cast<string_q *>(dataPtr);
        *str += (path+"\n");
    }
    return true;
}

//---------------------------------------------------------------------------
//void rebuildFourByteDB(void) {
//
//    string_q fileList;
//    string_q abiPath = getCachePath("abis/");
//    cout << abiPath << "\n";
//    forEveryFileInFolder(abiPath+"*", visitABIs, &fileList);
//
//    CFunctionArray funcArray;
//    while (!fileList.empty()) {
//        string_q fileName = nextTokenClear(fileList, '\n');
//        CAbi abi;
//        abi.loadAbiFromFile(fileName, false);
//        sort(abi.interfaces.begin(), abi.interfaces.end());
//        for (auto interface : abi.interfaces) {
//            funcArray.push_back(interface);
//            cout << interface.encoding << " : ";
//            cout << interface.name << " : ";
//            cout << interface.signature << "\n";
//        }
//    }
//    sort(funcArray.begin(), funcArray.end());
//    CArchive funcCache(WRITING_ARCHIVE);
//    if (funcCache.Lock(abiPath+"abis.bin", modeWriteCreate, LOCK_CREATE)) {
//        funcCache << funcArray;
//        funcCache.Release();
//    }
//}

//-----------------------------------------------------------------------
bool sortByFuncName(const CFunction& f1, const CFunction& f2) {
    string_q s1 = (f1.type == "event" ? "zzzevent" : f1.type) + f1.name + f1.encoding;
    for (auto f : f1.inputs) s1 += f.name;
    string_q s2 = (f2.type == "event" ? "zzzevent" : f2.type) + f2.name + f2.encoding;
    for (auto f : f2.inputs) s2 += f.name;
    return s1 < s2;
}
