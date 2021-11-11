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

import (
	"runtime"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/spf13/viper"
)

var blockScrapeViper = viper.New()
var blockScrapeRead = false
var cachedBlockScrape BlockScrape

type blockScrapeRequires struct {
	Tracing  bool
	Parity   bool
	Balances bool
	Archive  bool
}

type blockScrapeDev struct {
	IpfsGateway string `mapstructure:"ipfs_gateway"`
	MaxPoolSize int
}

type blockScrapeUnchainedIndex struct {
	Address              string
	ManifestHashEncoding string
}

type BlockScrape struct {
	Requires       blockScrapeRequires
	Dev            blockScrapeDev
	UnchainedIndex blockScrapeUnchainedIndex
}

// init sets up default values for the given configuration
func init() {
	blockScrapeViper.SetConfigName("blockScrape")
	blockScrapeViper.SetDefault("Dev.Ipfs_Gateway", "https://ipfs.unchainedindex.io/ipfs")
	blockScrapeViper.SetDefault("Dev.MaxPoolSize", runtime.NumCPU()*2)
	blockScrapeViper.SetDefault("UnchainedIndex.Address", "0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd")
	blockScrapeViper.SetDefault("UnchainedIndex.ManifestHashEncoding", "0x337f3f32")
	blockScrapeViper.SetDefault("Requires.Tracing", true)
	blockScrapeViper.SetDefault("Requires.Parity", true)
	blockScrapeViper.SetDefault("Requires.Balances", false)
}

// ReadBlockScrape reads the configuration located in blockScrape.toml file
func ReadBlockScrape() *BlockScrape {
	if !blockScrapeRead {
		MustReadConfig(blockScrapeViper, &cachedBlockScrape, false)

		// Validate the URL to ensure we have it in the correct format, so that ethClient.Dial
		// will not panic
		if !strings.HasPrefix(cachedBlockScrape.Dev.IpfsGateway, "http") {
			logger.Fatal("missing schema in IpfsGateway configuration: http or https")
		}

		blockScrapeRead = true
	}

	return &cachedBlockScrape
}
