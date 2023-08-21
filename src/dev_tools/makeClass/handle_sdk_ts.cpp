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

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_sdk_ts(void) {
    CStringArray types, paths;
    if (!handle_sdk_ts_types(types)) {
        return false;
    }

    if (!handle_sdk_ts_paths(paths)) {
        return false;
    }

    types.push_back("Basetypes");
    sort(types.begin(), types.end());
    ostringstream os1;
    for (auto type : types)
        os1 << "export * from './" << firstLower(type) << "';" << endl;
    os1 << "export * from './upgrades';" << endl;
    string_q typesPath = sdkPath + "typescript/src/types/index.ts";
    writeIfDifferent(typesPath, os1.str());

    sort(paths.begin(), paths.end());
    ostringstream os2;
    for (auto path : paths) {
        os2 << "export * from './" << path << "';" << endl;
    }
    string_q pathsPath = sdkPath + "typescript/src/paths/index.ts";
    writeIfDifferent(pathsPath, os2.str());

    return true;
}
