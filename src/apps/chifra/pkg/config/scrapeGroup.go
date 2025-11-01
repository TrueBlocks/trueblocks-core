// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/configtypes"
)

// GetScrape returns the scraper settings per chain
func GetScrape(chain string) configtypes.ScrapeSettings {
	return GetRootConfig().Chains[chain].Scrape
}

func SetScrapeArgs(chain string, args map[string]string) {
	ch := trueBlocksConfig.Chains[chain]

	empty := configtypes.ScrapeSettings{}
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
