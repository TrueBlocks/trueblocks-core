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

extern const char* STR_FORMAT_FUNCDATA;
//-----------------------------------------------------------------------
void handle_display(COptions& options) {

    CFunctionArray functions;
    for (auto addr : options.addrs)
        acquireABI(functions, addr, options, false);
    if (options.loadKnown) {
        CAbi abi;
        abi.loadKnownABIs();
        for (auto func : abi.abiByEncoding)
            functions.push_back(func);
    }

    string_q format = getGlobalConfig()->getDisplayStr(false, STR_FORMAT_FUNCDATA);
    string_q header = substitute(substitute(format, "[{", ""), "}]", "");
    if (options.asData)
        cout << header << "\n";
    replace(format, "[{address}]", toLower(options.primaryAddr));

    if (options.asData) {
        for (auto func : functions) {
            HIDE_FIELD(CFunction, "inputs");
            HIDE_FIELD(CFunction, "outputs");
            if (!func.constant || !options.noconst) {
                cout << func.Format(format) << "\n";
            }
        }

    } else {
        // print to a buffer because we have to modify it before we print it
        cout << "ABI for address " << options.primaryAddr;
        cout << (options.addrs.size() > 1 ? " and others" : "") << "\n";
        for (auto func : functions) {
            if (!func.constant || !options.noconst)
                cout << func.getSignature(options.parts) << "\n";
        }
        cout << "\n";
    }
}

//-----------------------------------------------------------------------
const char* STR_FORMAT_FUNCDATA =
"[{name}]\t"
"[{type}]\t"
"[{anonymous}]\t"
"[{constant}]\t"
"[{payable}]\t"
"[{signature}]\t"
"[{encoding}]\t"
"[{inputs}]\t"
"[{outputs}]\n";
