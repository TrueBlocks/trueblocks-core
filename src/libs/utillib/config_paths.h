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
extern string_q getPathToChainConfig_old(const string_q& _part);
extern string_q getPathToChainConfig_new(const string_q& _part);
extern string_q getPathToCache(const string_q& _part);
extern string_q getPathToIndex(const string_q& _part);

//--------------------------------------------------------------------------------
#define rootConfigs (getPathToRootConfig(""))
#define rootConfigs_abis (rootConfigs + "abis/")
#define rootConfigs_mocked (rootConfigs + "mocked/")

//-------------------------------------------------------------------------
#define chainConfigs_old (getPathToChainConfig_old(""))
#define chainConfigs_names (chainConfigs_old + "names/")
#define chainConfigs_manifest (chainConfigs_old + "manifest/")
#define chainConfigs_prices (chainConfigs_old + "")

//-------------------------------------------------------------------------
#define chainConfigs_new (getPathToChainConfig_new(""))

//-------------------------------------------------------------------------
#define cacheFolder (getPathToCache(""))
#define cacheFolder_abis (cacheFolder + "abis/")
#define cacheFolder_blocks (cacheFolder + "blocks/")
#define cacheFolder_monitors (cacheFolder + "monitors/")
#define cacheFolder_names (cacheFolder + "names/")
#define cacheFolder_objs (cacheFolder + "objs/")
#define cacheFolder_prices (cacheFolder + "prices/")
#define cacheFolder_recons (cacheFolder + "recons/")
#define cacheFolder_slurps (cacheFolder + "slurps/")
#define cacheFolder_tmp (cacheFolder + "tmp/")
#define cacheFolder_traces (cacheFolder + "traces/")
#define cacheFolder_txs (cacheFolder + "txs/")

//-------------------------------------------------------------------------
#define indexFolder (getPathToIndex(""))
#define indexFolder_staging (indexFolder + "staging/")
#define indexFolder_unripe (indexFolder + "unripe/")
#define indexFolder_ripe (indexFolder + "ripe/")
#define indexFolder_finalized (indexFolder + "finalized/")
#define indexFolder_blooms (indexFolder + "blooms/")
#define indexFolder_map (indexFolder + "maps/")

//---------------------------------------------------------------------------
#define chainFolderTxt_allocs (chainConfigs_new + "allocs.csv")
#define chainFolderTxt_names (chainConfigs_names + "names.tab")
#define chainFolderTxt_namesCustom (chainConfigs_names + "names_custom.tab")
#define chainFolderTxt_collections (chainConfigs_names + "collections.csv")
#define chainFolderTxt_manifest (chainConfigs_manifest + "manifest.txt")

//---------------------------------------------------------------------------
#define chainFolderZip_prices (chainConfigs_prices + "poloniex_USDT_ETH.bin.gz")
#define chainFolderZip_ts (chainConfigs_old + "ts.bin.gz")

//---------------------------------------------------------------------------
#define rootConfigToml_trueBlocks (rootConfigs + "trueBlocks.toml")
#define rootConfigToml_makeClass (rootConfigs + "makeClass.toml")

//---------------------------------------------------------------------------
#define cacheFolderBin_allocs (cacheFolder_names + "allocs.bin")
#define cacheFolderBin_names (cacheFolder_names + "names.bin")
#define cacheFolderBin_prices (cacheFolder_prices + "poloniex_USDT_ETH.bin")
#define indexFolderBin_ts (indexFolder + "ts.bin")

//---------------------------------------------------------------------------
extern string_q getPathToCommands(const string_q& _part);

}  // namespace qblocks
