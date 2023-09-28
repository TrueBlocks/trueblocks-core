// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// ScrapeSettings carries config information for the scraper
type ScrapeSettings struct {
	AppsPerChunk uint64 `toml:"appsPerChunk" json:"appsPerChunk"`
	SnapToGrid   uint64 `toml:"snapToGrid" json:"snapToGrid"`
	FirstSnap    uint64 `toml:"firstSnap" json:"firstSnap"`
	UnripeDist   uint64 `toml:"unripeDist" json:"unripeDist"`
	AllowMissing bool   `toml:"allowMissing" json:"allowMissing"`
	ChannelCount uint64 `toml:"channelCount" json:"channelCount"`
}

// GetScrape returns the scraper settings per chain
func GetScrape(chain string) ScrapeSettings {
	empty := ScrapeSettings{}
	if GetRootConfig().Chains[chain].Scrape == empty {
		settings := ScrapeSettings{
			AppsPerChunk: 2000000,
			SnapToGrid:   250000,
			FirstSnap:    2000000,
			UnripeDist:   28,
			ChannelCount: 20,
			AllowMissing: false,
		}
		if chain == "mainnet" {
			settings.SnapToGrid = 100000
			settings.FirstSnap = 2300000
		}
		ch := GetRootConfig().Chains[chain]
		ch.Scrape = settings
		GetRootConfig().Chains[chain] = ch
	}

	return GetRootConfig().Chains[chain].Scrape
}

func (s *ScrapeSettings) TestLog(chain string, test bool) {
	logger.TestLog(false, "AppsPerChunk: ", s.AppsPerChunk)
	logger.TestLog(false, "SnapToGrid: ", s.SnapToGrid)
	logger.TestLog(false, "FirstSnap: ", s.FirstSnap)
	logger.TestLog(false, "UnripeDist: ", s.UnripeDist)
	logger.TestLog(false, "ChannelCount: ", s.ChannelCount)
	logger.TestLog(false, "AllowMissing: ", s.AllowMissing)
}

func SetScrapeArgs(chain string, args map[string]string) {
	ch := trueBlocksConfig.Chains[chain]

	empty := ScrapeSettings{}
	if trueBlocksConfig.Chains[chain].Scrape == empty {
		ch.Scrape = GetScrape(chain)

	} else {
		settings := trueBlocksConfig.Chains[chain].Scrape
		for key, value := range args {
			switch key {
			case "appsPerChunk":
				settings.AppsPerChunk = utils.MustParseUint(value)
			case "snapToGrid":
				settings.SnapToGrid = utils.MustParseUint(value)
			case "firstSnap":
				settings.FirstSnap = utils.MustParseUint(value)
			case "unripeDist":
				settings.UnripeDist = utils.MustParseUint(value)
			case "channelCount":
				settings.ChannelCount = utils.MustParseUint(value)
			case "allowMissing":
				settings.AllowMissing = true
			}
		}
		ch.Scrape = settings
	}

	trueBlocksConfig.Chains[chain] = ch
}
