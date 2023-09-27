// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"os"
	"path/filepath"
	"reflect"
	"strconv"
	"strings"

	"github.com/BurntSushi/toml"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// TODO: We need a better way to handle configuration files
// TODO: See issue #2259 and OLD_BLOCKSCRAPE_CONFIG_CODE below

// ScrapeSettings are configuration items needed by the scraper
type ScrapeSettings struct {
	AppsPerChunk uint64 `json:"appsPerChunk"`           // The number of appearances to build into a chunk before consolidating it
	SnapToGrid   uint64 `json:"snapToGrid"`             // An override to apps_per_chunk to snap-to-grid at every modulo of this value, this allows easier corrections to the index
	FirstSnap    uint64 `json:"firstSnap"`              // The first block at which snap_to_grid is enabled
	UnripeDist   uint64 `json:"unripeDist"`             // The distance (in blocks) from the front of the chain under which (inclusive) a block is considered unripe
	ChannelCount uint64 `json:"-"`                      // Number of concurrent block processing channels
	AllowMissing bool   `json:"allowMissing,omitempty"` // Do not report errors for blockchain that contain blocks with zero addresses
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultSettings = ScrapeSettings{
	AppsPerChunk: 200000,
	SnapToGrid:   100000,
	FirstSnap:    0,
	UnripeDist:   28,
	ChannelCount: 20,
	AllowMissing: false,
	// EXISTING_CODE
	// EXISTING_CODE
}

var Unset = ScrapeSettings{
	AppsPerChunk: utils.NOPOS,
	SnapToGrid:   utils.NOPOS,
	FirstSnap:    utils.NOPOS,
	UnripeDist:   utils.NOPOS,
	ChannelCount: utils.NOPOS,
	AllowMissing: false,
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *ScrapeSettings) isDefault(chain, fldName string) bool {
	def := GetDefault(chain)
	switch fldName {
	case "AppsPerChunk":
		return s.AppsPerChunk == def.AppsPerChunk
	case "SnapToGrid":
		return s.SnapToGrid == def.SnapToGrid
	case "FirstSnap":
		return s.FirstSnap == def.FirstSnap
	case "UnripeDist":
		return s.UnripeDist == def.UnripeDist
	case "ChannelCount":
		return s.ChannelCount == def.ChannelCount
	case "AllowMissing":
		return s.AllowMissing == def.AllowMissing
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return false
}

func (s *ScrapeSettings) TestLog(chain string, test bool) {
	logger.TestLog(!s.isDefault(chain, "AppsPerChunk"), "AppsPerChunk: ", s.AppsPerChunk)
	logger.TestLog(!s.isDefault(chain, "SnapToGrid"), "SnapToGrid: ", s.SnapToGrid)
	logger.TestLog(!s.isDefault(chain, "FirstSnap"), "FirstSnap: ", s.FirstSnap)
	logger.TestLog(!s.isDefault(chain, "UnripeDist"), "UnripeDist: ", s.UnripeDist)
	logger.TestLog(!s.isDefault(chain, "ChannelCount"), "ChannelCount: ", s.ChannelCount)
	logger.TestLog(!s.isDefault(chain, "AllowMissing"), "AllowMissing: ", s.AllowMissing)
	// EXISTING_CODE
	// EXISTING_CODE
}

func GetDefault(chain string) ScrapeSettings {
	base := defaultSettings
	// EXISTING_CODE
	if chain == "mainnet" {
		base.AppsPerChunk = 2000000
		base.FirstSnap = 2300000
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
		configFn = filepath.Join(PathToRootConfig(), configFn)
	} else {
		configFn = filepath.Join(MustGetPathToChainConfig(chain), configFn)
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
	if !overlay.isDefault(chain, "AppsPerChunk") && overlay.AppsPerChunk != 0 && overlay.AppsPerChunk != utils.NOPOS {
		s.AppsPerChunk = overlay.AppsPerChunk
	}
	if !overlay.isDefault(chain, "SnapToGrid") && overlay.SnapToGrid != 0 && overlay.SnapToGrid != utils.NOPOS {
		s.SnapToGrid = overlay.SnapToGrid
	}
	if !overlay.isDefault(chain, "FirstSnap") && overlay.FirstSnap != 0 && overlay.FirstSnap != utils.NOPOS {
		s.FirstSnap = overlay.FirstSnap
	}
	if !overlay.isDefault(chain, "UnripeDist") && overlay.UnripeDist != 0 && overlay.UnripeDist != utils.NOPOS {
		s.UnripeDist = overlay.UnripeDist
	}
	if !overlay.isDefault(chain, "ChannelCount") && overlay.ChannelCount != 0 && overlay.ChannelCount != utils.NOPOS {
		s.ChannelCount = overlay.ChannelCount
	}
	if !overlay.isDefault(chain, "AllowMissing") && overlay.AllowMissing {
		s.AllowMissing = overlay.AllowMissing
	}

	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func AllowMissing(chain string) bool {
	s, _ := GetSettings(chain, "blockScrape.toml", nil)
	return s.AllowMissing
}

func toEnvStr(name string) string {
	return "TB_SETTINGS_" + strings.ToUpper(strings.Replace(name, "_", "", -1))
}

/*
OLD_BLOCKSCRAPE_CONFIG_CODE
type blockScrapeSettings struct {
	Block_chan_cnt        int
	Addr_chan_cnt         int
	AppsPerChunk        int
	UnripeDist           int
	SnapToGrid          int
	FirstSnap            int
	AllowMissing         bool
	Pinata_api_key        string
	Pinata_secret_api_key string
	Pinata_jwt            string
}

var defaultSettings = blockScrapeSettings{
	Block_chan_cnt:        10,
	Addr_chan_cnt:         20,
	AppsPerChunk:        200000,
	UnripeDist:           28,
	SnapToGrid:          100000,
	FirstSnap:            0,
	AllowMissing:         false,
	Pinata_api_key:        "",
	Pinata_secret_api_key: "",
	Pinata_jwt:            "",
}

type BlockScrape struct {
	Settings blockScrapeSettings
}

func GetBlockScrapeSettings(chain string) blockScrapeSettings {
	str := utils.AsciiFileToString(filepath.Join(MustGetPathToChainConfig(chain), "blockScrape.toml"))
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
