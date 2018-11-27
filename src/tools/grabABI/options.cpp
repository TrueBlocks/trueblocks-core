/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
static COption params[] = {
    COption("~addr",      "the address(es) of the smart contract(s) to grab"),
    COption("-canonical", "convert all types to their canonical represenation and remove all spaces from display"),
    COption("-generate",  "generate C++ code into the current folder for all functions and events found in the ABI"),
    COption("-data",      "export the display as data"),
    COption("-encode",    "generate the encodings for the functions / events in the ABI"),
    COption("-noconst",   "generate encodings for non-constant functions and events only (always true when generating)"), // NOLINT
    COption("-open",      "open the ABI file for editing, download if not already present"),
    COption("-raw",       "force retrieval of ABI from etherscan (ignoring cache)"),
    COption("@json",      "print the ABI to the screen as json"),
    COption("@silent",    "if ABI cannot be acquired, fail silently (useful for scripting)"),
    COption("@nodec",     "do not decorate duplicate names"),
    COption("@freshen",   "regenerate the binary database version of all ABIs in the abi cache"),
    COption("",           "Fetches the ABI for a smart contract. Optionally generates C++ source code "
                          "representing that ABI.\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        if (arg == "-g" || arg == "--gen" || arg == "--generate") {
            classDir = getCWD();
            prefix = getPrefix(classDir);

        } else if (arg == "-c" || arg == "--canonical") {
            if (parts&SIG_ENCODE)
                parts = (SIG_CANONICAL|SIG_ENCODE);
            else
                parts = SIG_CANONICAL;

        } else if (arg == "-e" || arg == "--encode") {
            parts |= SIG_ENCODE;

        } else if (arg == "-d" || arg == "--data") {
            parts |= SIG_FTYPE;
            asData = true;
            colorsOff();

        } else if (arg == "-s" || arg == "--silent") {
            silent = true;

        } else if (arg == "-n" || arg == "--noconst") {
            noconst = true;

        } else if (arg == "-f" || arg == "--freshen") {
extern void rebuildFourByteDB(void);
            rebuildFourByteDB();
            exit(0);

        } else if (arg == "--nodec") {
            decNames = false;

        } else if (arg == "-r" || arg == "--raw") {
            raw = true;

        } else if (arg == "-o" || arg == "--open") {
            open = true;

        } else if (arg == "-j" || arg == "--json") {
            asJson = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (primaryAddr.empty())
                primaryAddr = arg;
            if (!isAddress(arg) && arg != "0xTokenLib" && arg != "0xWalletLib")
                return usage("Invalid address '" + arg + "'. Length is not equal to 40 characters (20 bytes).\n");
            address_t addr = str_2_Addr(arg);
            if (arg == "0xTokenLib" || arg == "0xWalletLib") {
                addr = arg;
                addrs.push_back(addr);
            } else {
                addrs.push_back(toLower(addr));
            }
        }
    }

    if (parts == 0)
        parts = SIG_DEFAULT;

    if (parts != SIG_CANONICAL && verbose)
        parts |= SIG_DETAILS;

    if (!addrs.size())
        return usage("Please supply at least one Ethereum address.\n");

    bool isGenerate = !classDir.empty();
    if (isGenerate && asData)
        return usage("Incompatible options --generate and --data. Quitting...");

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    parts = SIG_DEFAULT;
    noconst = false;
    open = false;
    silent = false;
    asJson = false;
    raw = false;
    decNames = true;
    asData = false;
    addrs.clear();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options") {
        return str;

    } else if (which == "notes" && (verbose || COptions::isReadme)) {
        string_q ret;
        ret += "Use the [{--silent}] option, which displays fewer messages, for scripting.\n";
        return ret;
    }
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
void rebuildFourByteDB(void) {

    string_q fileList;
    string_q abiPath = blockCachePath("abis/");
    cout << abiPath << "\n";
    forEveryFileInFolder(abiPath+"*", visitABIs, &fileList);

    CFunctionArray funcArray;
    while (!fileList.empty()) {
        string_q fileName = nextTokenClear(fileList, '\n');
        CAbi abi;
        abi.loadABIFromFile(fileName);
        for (size_t f = 0 ; f < abi.abiByEncoding.size() ; f++) {
            funcArray.push_back(abi.abiByEncoding[f]);
            cout << abi.abiByEncoding[f].encoding << " : ";
            cout << abi.abiByEncoding[f].name << " : ";
            cout << abi.abiByEncoding[f].signature << "\n";
        }
    }
    sort(funcArray.begin(), funcArray.end());
    CArchive funcCache(WRITING_ARCHIVE);
    if (funcCache.Lock(abiPath+"abis.bin", binaryWriteCreate, LOCK_CREATE)) {
        funcCache << funcArray;
        funcCache.Release();
    }
}
