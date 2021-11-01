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
package config

// Functions in this package read and cache configuration files,
// filling in default values

var cachedGlobal Global
var DefaultIndexPath = GetConfigPath("unchained")

// ReadGlobal reads and caches the configuration located in trueBlocks.toml file
func ReadGlobal() *Global {
	if cachedGlobal.Version.Current != "" {
		return &cachedGlobal
	}

	config := &Global{
		Settings: globalSettings{
			IndexPath: DefaultIndexPath,
		},
	}

	ReadTo(config, "trueBlocks")
	cachedGlobal = *config
	return config
}

var cachedBlockScrape BlockScrape

// ReadBlockScrape reads and caches the configuration located in blockScrape.toml file
func ReadBlockScrape() *BlockScrape {
	if cachedBlockScrape.Dev.IpfsGateway != "" {
		return &cachedBlockScrape
	}

	config := &BlockScrape{
		Dev: blockScrapeDev{
			IpfsGateway: "https://ipfs.unchainedindex.io/ipfs",
		},
		Requires: blockScrapeRequires{
			Tracing:  true,
			Parity:   true,
			Balances: false,
		},
	}

	ReadTo(config, "blockScrape")
	cachedBlockScrape = *config
	return config
}
