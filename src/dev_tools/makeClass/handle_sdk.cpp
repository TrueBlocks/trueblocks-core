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
#include "utillib.h"
#include "options.h"

extern bool sortByDataModelName2(const CClassDefinition& c1, const CClassDefinition& c2);
//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk(void) {
    CToml config(rootConfigToml_makeClass);
    bool enabled = config.getConfigBool("enabled", "sdk", false);
    if (isTestMode() || !enabled) {
        LOG_WARN("Skipping sdk generation...");
        return true;
    }

    LOG_INFO(cYellow, "handling sdk generation...", cOff);
    sort(dataModels.begin(), dataModels.end(), sortByDataModelName2);

    if (!handle_sdk_ts()) {
        return false;
    }

    if (!handle_sdk_py()) {
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------
bool sortByDataModelName2(const CClassDefinition& c1, const CClassDefinition& c2) {
    return c1.class_name < c2.class_name;
}
