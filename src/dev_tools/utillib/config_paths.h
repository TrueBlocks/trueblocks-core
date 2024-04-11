#pragma once
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
#include "basetypes.h"
#include "filenames.h"

//-----------------------------------------------------------------------------
namespace qblocks {

//--------------------------------------------------------------------------------
extern string_q getPathToRootConfig(const string_q& _part);
extern string_q getPathToChainConfig(const string_q& _part);
extern string_q getPathToCache(const string_q& _part);
extern string_q getPathToIndex(const string_q& _part);
extern string_q getChain(void);

//--------------------------------------------------------------------------------
#define rootConfigs (getPathToRootConfig(""))
#define rootConfigs_abis (rootConfigs + "abis/")

//-------------------------------------------------------------------------
#define chainConfigs (getPathToChainConfig(""))

//-------------------------------------------------------------------------
#define cacheFolder (getPathToCache(""))
#define cacheFolder_tmp (cacheFolder + "tmp/")

//---------------------------------------------------------------------------
#define chainConfigsTxt_specials (chainConfigs + "specials.csv")

//---------------------------------------------------------------------------
#define rootConfigToml_trueBlocks (rootConfigs + "trueBlocks.toml")
#define rootConfigToml_makeClass (rootConfigs + "makeClass.toml")
#define rootConfigToml_merge (rootConfigs + name + ".toml")
#define chainConfigToml_merge (chainConfigs + name + ".toml")

//---------------------------------------------------------------------------
extern string_q getPathToCommands(const string_q& _part);
extern void loadEnvironmentPaths(void);

}  // namespace qblocks
