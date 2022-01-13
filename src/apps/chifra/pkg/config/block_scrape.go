// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

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
	Tracing bool
	Parity  bool
	Archive bool
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
}

// ReadBlockScrape reads the configuration located in blockScrape.toml file
func ReadBlockScrape() *BlockScrape {
	if !blockScrapeRead {
		MustReadConfig(blockScrapeViper, &cachedBlockScrape, GetPathToChainConfig_newOff(), false)

		// Validate the URL to ensure we have it in the correct format, so that ethClient.Dial
		// will not panic
		if !strings.HasPrefix(cachedBlockScrape.Dev.IpfsGateway, "http") {
			logger.Fatal("missing schema in IpfsGateway configuration: http or https")
		}

		blockScrapeRead = true
	}

	return &cachedBlockScrape
}
