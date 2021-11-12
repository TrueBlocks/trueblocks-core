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

//-----------------------------------------------------------------------
void COptions::handle_convertsol(const address_t& addr) {
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
