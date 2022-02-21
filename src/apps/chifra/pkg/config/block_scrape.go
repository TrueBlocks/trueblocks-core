// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"github.com/spf13/viper"
)

// BOGUS TODO: This needs to be per-chain -- see the timestamps cached-in-memory data
var blockScrapeViper = viper.New()
var blockScrapeRead = false
var cachedBlockScrape BlockScrape

type blockScrapeRequires struct {
	Tracing bool
	Parity  bool
	Archive bool
}

type blockScrapeUnchainedIndex struct {
	Address              string
	ManifestHashEncoding string
}

type BlockScrape struct {
	Requires       blockScrapeRequires
	UnchainedIndex blockScrapeUnchainedIndex
}

// init sets up default values for the given configuration
func init() {
	blockScrapeViper.SetConfigName("blockScrape")
	blockScrapeViper.SetDefault("UnchainedIndex.Address", "0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd")
	blockScrapeViper.SetDefault("UnchainedIndex.ManifestHashEncoding", "0x337f3f32")
	blockScrapeViper.SetDefault("Requires.Tracing", true)
	blockScrapeViper.SetDefault("Requires.Parity", true)
}

// ReadBlockScrape reads the configuration located in blockScrape.toml file
func ReadBlockScrape(chain string) *BlockScrape {
	if !blockScrapeRead {
		MustReadConfig(blockScrapeViper, &cachedBlockScrape, GetPathToChainConfig(chain)+"blockScrape.toml")
		blockScrapeRead = true
	}

	return &cachedBlockScrape
}
