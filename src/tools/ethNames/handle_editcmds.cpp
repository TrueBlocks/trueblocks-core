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
#define LOGGING_LEVEL_TEST
#include "options.h"
#include "exportcontext.h"

//-----------------------------------------------------------------------
bool COptions::handle_editcmds(bool autoname) {
    string_q crud = crudCommands[0];
    if (!contains("create|update|delete|undelete|remove", crud))
        return usage("Invalid edit command '" + crud + "'.");

    target.address = toLower(trim(getEnvStr("TB_NAME_ADDRESS"), '\"'));
    target.name = trim(getEnvStr("TB_NAME_NAME"), '\"');
    target.tags = trim(getEnvStr("TB_NAME_TAG"), '\"');
    target.source = trim(getEnvStr("TB_NAME_SOURCE"), '\"');
    target.symbol = trim(getEnvStr("TB_NAME_SYMBOL"), '\"');
    target.decimals = str_2_Uint(trim(getEnvStr("TB_NAME_DECIMALS"), '\"'));
    target.description = trim(getEnvStr("TB_NAME_DESCR"), '\"');
    target.isCustom = str_2_Bool(trim(getEnvStr("TB_NAME_CUSTOM"), '\"')) || to_custom;
    // for delete and remove, we pick it up from the command line
    if (target.address.empty()) {
        if (terms.size() == 0)
            return false;
        target.address = terms[0];
    }
    finishClean(target);
    terms.clear();
    terms.push_back(target.address);  // we only need the address for the search

    return updateName(target, crud);
}
