// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package scrape

import (
	"log"
	"os"
	"reflect"
	"strconv"
	"strings"

	"github.com/BurntSushi/toml"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type scrapeSettings struct {
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

var defaultSettings = scrapeSettings{
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

type ScrapeConfig struct {
	Settings scrapeSettings
}

func GetSettings(chain string) scrapeSettings {
	str := utils.AsciiFileToString(config.GetPathToChainConfig(chain) + "blockScrape.toml")
	conf := ScrapeConfig{
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
			ff := utils.MakeFirstUpperCase(field)
			if strings.HasPrefix(field, "pinata") {
				reflect.ValueOf(&conf.Settings).Elem().FieldByName(ff).SetString(val)
			} else if field == "allow_missing" {
				if val == "true" {
					reflect.ValueOf(&conf.Settings).Elem().FieldByName(ff).SetBool(true)
				}
			} else {
				if v, err := strconv.ParseInt(val, 10, 32); err == nil {
					reflect.ValueOf(&conf.Settings).Elem().FieldByName(ff).SetInt(v)
				}
			}
		}
	}

	return conf.Settings
}

func AllowMissing(chain string) bool {
	return GetSettings(chain).Allow_missing
}

func PinataKeys(chain string) (string, string) {
	return GetSettings(chain).Pinata_api_key, GetSettings(chain).Pinata_secret_api_key
}
