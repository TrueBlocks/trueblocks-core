// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// ScrapeSettings carries config information for the scraper
type ScrapeSettings struct {
	AppsPerChunk uint64 `toml:"appsPerChunk" json:"appsPerChunk"`
	SnapToGrid   uint64 `toml:"snapToGrid" json:"snapToGrid"`
	FirstSnap    uint64 `toml:"firstSnap" json:"firstSnap"`
	UnripeDist   uint64 `toml:"unripeDist" json:"unripeDist"`
	AllowMissing bool   `toml:"allowMissing" json:"allowMissing,omitempty"`
	ChannelCount uint64 `toml:"channelCount" json:"channelCount,omitempty"`
}

// GetScrape returns the scraper settings per chain
func GetScrape(chain string) ScrapeSettings {
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
				settings.AppsPerChunk, _ = strconv.ParseUint(value, 0, 64)
			case "snapToGrid":
				settings.SnapToGrid, _ = strconv.ParseUint(value, 0, 64)
			case "firstSnap":
				settings.FirstSnap, _ = strconv.ParseUint(value, 0, 64)
			case "unripeDist":
				settings.UnripeDist, _ = strconv.ParseUint(value, 0, 64)
			case "channelCount":
				settings.ChannelCount, _ = strconv.ParseUint(value, 0, 64)
			case "allowMissing":
				settings.AllowMissing = true
			}
		}
		ch.Scrape = settings
	}

	trueBlocksConfig.Chains[chain] = ch
}
