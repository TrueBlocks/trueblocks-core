package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"os"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// ScrapePreScrape is called each time around the forever loop prior to calling into
// Blaze to actually scrape the blocks.
func (opts *ScrapeOptions) ScrapePreScrape(progressThen *rpcClient.MetaData) (ok bool, err error) {

	if utils.OnOff {
		fmt.Println()
		fmt.Println("----------------------------------------------------------------------------------------------")
		logger.Log(logger.Info, "ScrapePreScrape", os.Getenv("TEST_END_SCRAPE"))
	}

	tes := os.Getenv("TEST_END_SCRAPE")
	val, err := strconv.ParseUint(tes, 10, 32)
	if (val != 0 && progressThen.Finalized > val) || err != nil {
		return false, err
	}

	return true, nil
}
