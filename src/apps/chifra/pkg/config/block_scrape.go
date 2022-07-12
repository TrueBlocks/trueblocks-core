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

// TODO: BOGUS - IT WOULD BE HELPFUL IF THIS MERGED IN DATA FROM THE MAINNET FILE FOR PIN APIS
func GetPinataKeys(chain string) (string, string) {
	settings := GetBlockScrapeSettings(chain)
	return settings.Pinata_api_key, settings.Pinata_secret_api_key
}
