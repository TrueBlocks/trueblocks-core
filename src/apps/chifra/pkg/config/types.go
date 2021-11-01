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

// Global config

type globalVersion struct {
	Current string
}

type globalSettings struct {
	RpcProvider  string
	CachePath    string
	IndexPath    string
	EtherscanKey string `toml:"etherscan_key"`
}

type Global struct {
	Version  globalVersion
	Settings globalSettings
}

// BlockScrape config

type blockScrapeRequires struct {
	Tracing  bool
	Parity   bool
	Balances bool
	Archive  bool
}

type blockScrapeDev struct {
	IpfsGateway string `toml:"ipfs_gateway"`
}

type BlockScrape struct {
	Requires blockScrapeRequires
	Dev      blockScrapeDev
}
