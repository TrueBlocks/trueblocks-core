// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"github.com/spf13/viper"
)

// TODO: Multi-chain configure individual tools per chain (for example blockScrape)
var blockScrapeViper = viper.New()
var blockScrapeRead = false
var cachedBlockScrape BlockScrape

type blockScrapeRequires struct {
	Tracing bool
	Parity  bool
	Archive bool
}

// We should remove these underlined config entries in a migration some day
type blockScrapeSettings struct {
	Block_chan_cnt        int
	Addr_chan_cnt         int
	Apps_per_chunk        int
	Unripe_dist           int
	Snap_to_grid          int
	First_snap            int
	Allow_missing         bool
	Pinata_api_key        string
	Pinata_secret_api_key string
	Pinata_jwt            string
}

type BlockScrape struct {
	Requires blockScrapeRequires
	Settings blockScrapeSettings
}

// init sets up default values for the given configuration
func init() {
	blockScrapeViper.SetConfigName("blockScrape")
	blockScrapeViper.SetDefault("Requires.Tracing", true)
	blockScrapeViper.SetDefault("Requires.Parity", true)
	blockScrapeViper.SetDefault("Settings.Block_chan_cnt", 10)
	blockScrapeViper.SetDefault("Settings.Addr_chan_cnt", 20)
	blockScrapeViper.SetDefault("Settings.Apps_per_chunk", 200000)
	blockScrapeViper.SetDefault("Settings.Unripe_dist", 28)
	blockScrapeViper.SetDefault("Settings.Snap_to_grid", 100000)
	blockScrapeViper.SetDefault("Settings.First_snap", 0)
	blockScrapeViper.SetDefault("Settings.Allow_missing", false)
}

// ReadBlockScrape reads the configuration located in blockScrape.toml file
func ReadBlockScrape(chain string) *BlockScrape {
	if !blockScrapeRead {
		MustReadConfig(blockScrapeViper, &cachedBlockScrape, GetPathToChainConfig(chain))
		blockScrapeRead = true
	}
	// fmt.Println("Block_chan_cnt:", cachedBlockScrape.Settings.Block_chan_cnt)
	// fmt.Println("Addr_chan_cnt:", cachedBlockScrape.Settings.Addr_chan_cnt)
	// fmt.Println("Apps_per_chunk:", cachedBlockScrape.Settings.Apps_per_chunk)
	// fmt.Println("Unripe_dist:", cachedBlockScrape.Settings.Unripe_dist)
	// fmt.Println("Snap_to_grid:", cachedBlockScrape.Settings.Snap_to_grid)
	// fmt.Println("First_snap:", cachedBlockScrape.Settings.First_snap)
	// fmt.Println("Allow_missing:", cachedBlockScrape.Settings.Allow_missing)
	return &cachedBlockScrape
}

func GetPinataKeys(chain string) (string, string) {
	// TODO: BOGUS - IT WOULD BE HELPFUL IF THIS MERGED IN DATA FROM THE MAINNET FILE FOR PIN APIS
	return ReadBlockScrape(chain).Settings.Pinata_api_key, ReadBlockScrape(chain).Settings.Pinata_secret_api_key
}
