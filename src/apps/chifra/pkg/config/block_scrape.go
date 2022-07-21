// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"log"
	"os"
	"reflect"
	"strconv"
	"strings"

	"github.com/BurntSushi/toml"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/viper"
)

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

var defaultSettings = blockScrapeSettings{
	Block_chan_cnt:        10,
	Addr_chan_cnt:         20,
	Apps_per_chunk:        200000,
	Unripe_dist:           28,
	Snap_to_grid:          100000,
	First_snap:            0,
	Allow_missing:         false,
	Pinata_api_key:        "",
	Pinata_secret_api_key: "",
	Pinata_jwt:            "",
}

type BlockScrape struct {
	Settings blockScrapeSettings
}

// TODO: Multi-chain configure individual tools per chain (for example blockScrape)
var blockScrapeViper = viper.New()
var blockScrapeRead = false
var cachedBlockScrape BlockScrape1

type blockScrapeRequires struct {
	Tracing bool
	Parity  bool
	Archive bool
}

type blockScrapeUnchainedIndex struct {
	Address              string
	ManifestHashEncoding string
}

type BlockScrape1 struct {
	Requires       blockScrapeRequires
	UnchainedIndex blockScrapeUnchainedIndex
	Settings       blockScrapeSettings
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
func ReadBlockScrape(chain string) *BlockScrape1 {
	if !blockScrapeRead {
		MustReadConfig(blockScrapeViper, &cachedBlockScrape, GetPathToChainConfig(chain))
		blockScrapeRead = true
	}

	return &cachedBlockScrape
}

func GetBlockScrapeSettings(chain string) blockScrapeSettings {
	str := utils.AsciiFileToString(GetPathToChainConfig(chain) + "blockScrape.toml")
	conf := BlockScrape{
		Settings: defaultSettings,
	}
	if _, err := toml.Decode(str, &conf); err != nil {
		// TODO: Don't panic here, just report and return defaults
		log.Fatal(err)
	}

	tt := reflect.TypeOf(conf.Settings)
	fields, _, _ := utils.GetFields(&tt, "txt", true)

	for _, field := range fields {
		key := "TB_SETTINGS_" + strings.ToUpper(field)
		val := os.Getenv(key)
		if val != "" {
			if strings.HasPrefix(field, "pinata") {
				reflect.ValueOf(&conf.Settings).Elem().FieldByName(utils.MakeFirstUpperCase(field)).SetString(val)
			} else if field == "allow_missing" {
				if val == "true" {
					reflect.ValueOf(&conf.Settings).Elem().FieldByName(utils.MakeFirstUpperCase(field)).SetBool(true)
				}
			} else {
				if v, err := strconv.ParseInt(val, 10, 32); err == nil {
					reflect.ValueOf(&conf.Settings).Elem().FieldByName(utils.MakeFirstUpperCase(field)).SetInt(v)
				}
			}
		}
	}

	return conf.Settings
}
