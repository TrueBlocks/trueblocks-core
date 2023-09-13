// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package scrapeCfg

import (
	"os"
	"path/filepath"
	"reflect"
	"strconv"
	"strings"

	"github.com/BurntSushi/toml"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// TODO: We need a better way to handle configuration files
// TODO: See issue #2259 and OLD_BLOCKSCRAPE_CONFIG_CODE below

// ScrapeSettings are configuration items needed by the scraper
type ScrapeSettings struct {
	Apps_per_chunk uint64 `json:"appsPerChunk"`           // The number of appearances to build into a chunk before consolidating it
	Snap_to_grid   uint64 `json:"snapToGrid"`             // An override to apps_per_chunk to snap-to-grid at every modulo of this value, this allows easier corrections to the index
	First_snap     uint64 `json:"firstSnap"`              // The first block at which snap_to_grid is enabled
	Unripe_dist    uint64 `json:"unripeDist"`             // The distance (in blocks) from the front of the chain under which (inclusive) a block is considered unripe
	Channel_count  uint64 `json:"-"`                      // Number of concurrent block processing channels
	Allow_missing  bool   `json:"allowMissing,omitempty"` // Do not report errors for blockchain that contain blocks with zero addresses
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultSettings = ScrapeSettings{
	Apps_per_chunk: 200000,
	Snap_to_grid:   100000,
	First_snap:     0,
	Unripe_dist:    28,
	Channel_count:  20,
	Allow_missing:  false,
	// EXISTING_CODE
	// EXISTING_CODE
}

var Unset = ScrapeSettings{
	Apps_per_chunk: utils.NOPOS,
	Snap_to_grid:   utils.NOPOS,
	First_snap:     utils.NOPOS,
	Unripe_dist:    utils.NOPOS,
	Channel_count:  utils.NOPOS,
	Allow_missing:  false,
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *ScrapeSettings) isDefault(chain, fldName string) bool {
	def := GetDefault(chain)
	switch fldName {
	case "Apps_per_chunk":
		return s.Apps_per_chunk == def.Apps_per_chunk
	case "Snap_to_grid":
		return s.Snap_to_grid == def.Snap_to_grid
	case "First_snap":
		return s.First_snap == def.First_snap
	case "Unripe_dist":
		return s.Unripe_dist == def.Unripe_dist
	case "Channel_count":
		return s.Channel_count == def.Channel_count
	case "Allow_missing":
		return s.Allow_missing == def.Allow_missing
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return false
}

func (s *ScrapeSettings) TestLog(chain string, test bool) {
	logger.TestLog(!s.isDefault(chain, "Apps_per_chunk"), "Apps_per_chunk: ", s.Apps_per_chunk)
	logger.TestLog(!s.isDefault(chain, "Snap_to_grid"), "Snap_to_grid: ", s.Snap_to_grid)
	logger.TestLog(!s.isDefault(chain, "First_snap"), "First_snap: ", s.First_snap)
	logger.TestLog(!s.isDefault(chain, "Unripe_dist"), "Unripe_dist: ", s.Unripe_dist)
	logger.TestLog(!s.isDefault(chain, "Channel_count"), "Channel_count: ", s.Channel_count)
	logger.TestLog(!s.isDefault(chain, "Allow_missing"), "Allow_missing: ", s.Allow_missing)
	// EXISTING_CODE
	// EXISTING_CODE
}

func GetDefault(chain string) ScrapeSettings {
	base := defaultSettings
	// EXISTING_CODE
	if chain == "mainnet" {
		base.Apps_per_chunk = 2000000
		base.First_snap = 2300000
	}
	// EXISTING_CODE
	return base
}

// GetSettings retrieves scrape config from (in order) default, config, environment, optionally provided cmdLine
func GetSettings(chain, configFn string, cmdLine *ScrapeSettings) (ScrapeSettings, error) {
	// Start with the defalt values...
	base := GetDefault(chain)

	tt := reflect.TypeOf(defaultSettings)
	fieldList, _, _ := utils.GetFields(&tt, "txt", true)

	if strings.Contains(configFn, "trueBlocks.toml") {
		configFn = filepath.Join(config.GetPathToRootConfig(), configFn)
	} else {
		configFn = filepath.Join(config.GetPathToChainConfig(chain), configFn)
	}

	if file.FileExists(configFn) {
		type TomlFile struct {
			Settings ScrapeSettings
		}
		var t TomlFile
		t.Settings = Unset
		// ...pick up values from toml file...
		if _, err := toml.Decode(file.AsciiFileToString(configFn), &t); err != nil {
			logger.Error("Could not load", configFn)
			logger.Fatal("Error:", err)
			return ScrapeSettings{}, err
		}
		base.overlay(chain, t.Settings)
	}

	// ...check the environment...
	for _, field := range fieldList {
		envKey := toEnvStr(field)
		envValue := os.Getenv(envKey)
		if envValue != "" {
			fName := utils.MakeFirstUpperCase(field)
			fld := reflect.ValueOf(&base).Elem().FieldByName(fName)
			if fld.Kind() == reflect.String {
				fld.SetString(envValue)
			} else if fld.Kind() == reflect.Bool {
				if envValue == "true" {
					fld.SetBool(true)
				}
			} else {
				if v, err := strconv.ParseUint(envValue, 10, 64); err == nil {
					fld.SetUint(v)
				}
			}
		}
	}

	if cmdLine != nil {
		base.overlay(chain, *cmdLine)
	}

	return base, nil
}

func (s *ScrapeSettings) overlay(chain string, overlay ScrapeSettings) {
	if !overlay.isDefault(chain, "Apps_per_chunk") && overlay.Apps_per_chunk != 0 && overlay.Apps_per_chunk != utils.NOPOS {
		s.Apps_per_chunk = overlay.Apps_per_chunk
	}
	if !overlay.isDefault(chain, "Snap_to_grid") && overlay.Snap_to_grid != 0 && overlay.Snap_to_grid != utils.NOPOS {
		s.Snap_to_grid = overlay.Snap_to_grid
	}
	if !overlay.isDefault(chain, "First_snap") && overlay.First_snap != 0 && overlay.First_snap != utils.NOPOS {
		s.First_snap = overlay.First_snap
	}
	if !overlay.isDefault(chain, "Unripe_dist") && overlay.Unripe_dist != 0 && overlay.Unripe_dist != utils.NOPOS {
		s.Unripe_dist = overlay.Unripe_dist
	}
	if !overlay.isDefault(chain, "Channel_count") && overlay.Channel_count != 0 && overlay.Channel_count != utils.NOPOS {
		s.Channel_count = overlay.Channel_count
	}
	if !overlay.isDefault(chain, "Allow_missing") && overlay.Allow_missing {
		s.Allow_missing = overlay.Allow_missing
	}

	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func toEnvStr(name string) string {
	return "TB_SETTINGS_" + strings.ToUpper(strings.Replace(name, "_", "", -1))
}

/*
OLD_BLOCKSCRAPE_CONFIG_CODE
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
		logger.Fatal(err)
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
*/
// EXISTING_CODE
