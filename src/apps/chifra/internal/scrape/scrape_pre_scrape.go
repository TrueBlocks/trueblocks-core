// TODO: BOGUS - THINGS TO DO:
// TODO: BOGUS - CLEANUP BETWEEN RUNS IF THINGS DON'T WORK OUT AND SOMETHING HAD TO QUIT EARLY
// TODO: BOGUS - MAKE SURE WE'RE NOT RUNNING IF ACCTSCRAPE (OR ANYTHING ELSE?) IS RUNNINGs
// TODO: BOGUS - RESPOND WELL TO RUNNING AGAINST NON-ARCHIVE NODES see isArchiveNode() in C++ code
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
)

func (opts *ScrapeOptions) preScrape(progressThen *rpcClient.MetaData) (bool, error) {

	fmt.Println()
	fmt.Println("----------------------------------------------------------------------------------------------")
	logger.Log(logger.Info, "PreScrape", os.Getenv("TEST_END_SCRAPE"))

	e := os.Getenv("TEST_END_SCRAPE")
	ee, _ := strconv.ParseUint(e, 10, 32)
	if ee != 0 && progressThen.Finalized > ee {
		return false, nil
	}

	return true, nil
}
