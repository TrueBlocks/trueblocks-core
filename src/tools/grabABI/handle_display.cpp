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

extern const char *STR_DISPLAY_ABIFUNCTION;
//-----------------------------------------------------------------------
void COptions::handle_display(void) {

    string_q format = getGlobalConfig("grabABI")->getConfigStr("display", "format", STR_DISPLAY_ABIFUNCTION);
    string_q header = toLower(substitute(substitute(format, "[{", ""), "}]", ""));
    if (asData)
        cout << header << "\n";
    replace(format, "[{address}]", toLower("")); //primaryAddr));

    for (auto abi : abi_specs) {
        if (asData) {
            HIDE_FIELD(CFunction, "inputs");
            HIDE_FIELD(CFunction, "outputs");
            for (auto interface : abi.interfaces) {
                if (!interface.constant || !noconst) {
                    cout << interface.Format(format) << "\n";
                }
            }
        } else {
            // print to a buffer because we have to modify it before we print it
            cout << "ABI for address " <<  abi.address << "\n";
            for (auto interface : abi.interfaces) {
                if (!interface.constant || !noconst)
                    cout << interface.getSignature(parts) << "\n";
            }
            cout << "\n";
        }
    }
}

//-----------------------------------------------------------------------
const char *STR_DISPLAY_ABIFUNCTION =
"[{ADDRESS}]\t"
"[{ENCODING}]\t"
"[{TYPE}]\t"
"[{CONSTANT}]\t"
"[{NAME}]\t"
"[{SIGNATURE}]\t"
"[{INPUT_NAMES}]";
