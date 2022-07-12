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

func (opts *ScrapeOptions) Y_2_preScrape(progressThen *rpcClient.MetaData) (bool, error) {

	// TODO: BOGUS - TESTING SCRAPING
	if utils.OnOff {
		fmt.Println()
		fmt.Println("----------------------------------------------------------------------------------------------")
		logger.Log(logger.Info, "PreScrape", os.Getenv("TEST_END_SCRAPE"))
	}

	e := os.Getenv("TEST_END_SCRAPE")
	ee, _ := strconv.ParseUint(e, 10, 32)
	if ee != 0 && progressThen.Finalized > ee {
		return false, nil
	}

	return true, nil
}
