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
#include "etherlib.h"
#include "options.h"

//-----------------------------------------------------------------------
bool addIfUnique(const string_q& addr, CFunctionArray& functions, CFunction& func, bool decorateNames) {
    if (func.name.empty())  // && func.type != "constructor")
        return false;

    for (auto f : functions) {
        if (f.encoding == func.encoding)
            return false;

        // different encoding same name means a duplicate function name in the code. We won't build with
        // duplicate function names, so we need to modify the incoming function. We do this by appending
        // the first four characters of the contract's address.
        if (decorateNames && f.name == func.name && !f.isBuiltin) {
            func.origName = func.name;
            func.name += (startsWith(addr, "0x") ? extract(addr, 2, 4) : extract(addr, 0, 4));
        }
    }

    functions.push_back(func);
    return true;
}

//-----------------------------------------------------------------------
string_q acquireABI(CFunctionArray& functions, const address_t& addr, const COptions& opt, bool builtIn) {

    string_q results;
    string_q fileName = blockCachePath("abis/" + addr + ".json");
    string_q localFile("./" + addr + ".json");
    if (fileExists(localFile)) {
        cerr << "Local file copied to cache\n";
        copyFile(localFile, fileName);
    }

    string_q dispName = substitute(fileName, configPath(""), "|");
    nextTokenClear(dispName, '|');
    dispName = "~/.quickBlocks/" + dispName;
    if (fileExists(fileName) && !opt.raw) {

        if (verbose && !isTestMode()) {
            cerr << "Reading ABI for " << addr << " from cache " + dispName + "\r";
            cerr.flush();
        }
        asciiFileToString(fileName, results);

    } else {
        if (verbose && !isTestMode()) {
            cerr << "Reading ABI for " << addr << " from EtherScan\r";
            cerr.flush();
        }
        string_q url = string_q("http:/")
        + "/api.etherscan.io/api?module=contract&action=getabi&address="
        + addr;
        results = substitute(urlToString(url), "\\", "");
        if (!contains(results, "NOTOK")) {
            // Clear the RPC wrapper
            replace(results, "{\"status\":\"1\",\"message\":\"OK\",\"result\":\"", "");
            replaceReverse(results, "]\"}", "");
            if (verbose) {
                if (!isTestMode())
                    cout << verbose << "---------->" << results << "\n";
                cout.flush();
            }
            nextTokenClear(results, '[');
            replaceReverse(results, "]}", "");
            if (!isTestMode()) {
                cerr << "Caching abi in " << dispName << "\n";
            }
            establishFolder(fileName);
            stringToAsciiFile(fileName, "["+results+"]");
        } else if (contains(results, "source code not verified")) {
            if (!opt.silent) {
                cerr << "\n";
                cerr << cRed << "Warning: " << cOff;
                cerr << "Failed to grab the ABI. Etherscan returned:\n\n\t";
                cerr << cTeal << results << cOff << "\n\n";
                cerr << "However, the ABI may actually be present on EtherScan. QBlocks will use it if\n";
                cerr << "you copy and paste the ABI json to this file:\n\n\t";
                cerr << cTeal << localFile << cOff << "\n\n";
                quickQuitHandler(0);
            }
        } else {
            if (!opt.silent) {
                cerr << "Etherscan returned " << results << "\n";
                cerr << "Could not grab ABI for " + addr + " from etherscan.io.\n";
                quickQuitHandler(0);
            }
            // TODO(tjayrush): If we store the ABI here even if empty, we won't have to get it again, but then
            // what happens if user later posts the ABI? Need a 'refresh' option or clear cache option
            establishFolder(fileName);
            stringToAsciiFile(fileName, "[]");
        }
    }

    CFunction func;
    ostringstream os;
    while (func.parseJson3(results)) {
        func.isBuiltin = builtIn;
        if (addIfUnique(addr, functions, func, opt.decNames))
            func.doExport(os);
        func = CFunction();  // reset
    }
    return os.str();
}
