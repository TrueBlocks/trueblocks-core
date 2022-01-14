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
#define rootConfigs (getPathToRootConfig(""))

//-------------------------------------------------------------------------
extern string_q getPathToChainConfig_newOff(const string_q& _part);
#define chainConfigs_old (getPathToChainConfig_newOff(""))
#define chainConfigs_names (getPathToChainConfig_newOff("names/"))
#define chainConfigs_manifest (getPathToChainConfig_newOff("manifest/"))
#define chainConfigs_prices (getPathToChainConfig_newOff(""))

//-------------------------------------------------------------------------
extern string_q getPathToChainConfig_new(const string_q& _part);
#define chainConfigs_new (getPathToChainConfig_new(""))

//-------------------------------------------------------------------------
extern string_q getPathToCache(const string_q& _part);
#define cacheFolder (getPathToCache(""))
#define cacheFolder_abis (getPathToCache("abis/"))
#define cacheFolder_blocks (getPathToCache("blocks/"))
#define cacheFolder_monitors (getPathToCache("monitors/"))
#define cacheFolder_names (getPathToCache("names/"))
#define cacheFolder_objs (getPathToCache("objs/"))
#define cacheFolder_prices (getPathToCache("prices/"))
#define cacheFolder_recons (getPathToCache("recons/"))
#define cacheFolder_slurps (getPathToCache("slurps/"))
#define cacheFolder_tmp (getPathToCache("tmp/"))
#define cacheFolder_traces (getPathToCache("traces/"))
#define cacheFolder_txs (getPathToCache("txs/"))

//-------------------------------------------------------------------------
extern string_q getPathToIndex(const string_q& _part);
#define indexFolder (getPathToIndex(""))
#define indexFolder_staging (getPathToIndex("staging/"))
#define indexFolder_unripe (getPathToIndex("unripe/"))
#define indexFolder_ripe (getPathToIndex("ripe/"))
#define indexFolder_finalized (getPathToIndex("finalized/"))
#define indexFolder_blooms (getPathToIndex("blooms/"))
#define indexFolder_map (getPathToIndex("maps/"))

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
