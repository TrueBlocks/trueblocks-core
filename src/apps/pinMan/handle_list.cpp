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
#include "options.h"

//----------------------------------------------------------------
bool COptions::handle_list(void) {
    if (remote) {
        cerr << "remote not implemented" << endl;

    } else {
        ASSERT(pins.size());  // local pins have already been loaded
        for (auto pin : pins) {
            if (!isJson()) {
                cout << trim(pin.Format(expContext().fmtMap["format"]), '\t') << endl;
            } else {
                cout << ((isJson() && !firstOut) ? ", " : "");
                indent();
                pin.toJson(cout);
                unindent();
            }
            firstOut = false;
        }
    }
    return false;
}
