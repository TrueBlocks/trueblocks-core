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
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ScrapeOptions) HandleScrape() error {
	// TODO: BOGUS - TESTING SCRAPING
	if os.Getenv("NO_COLOR") == "true" {
		colors.ColorsOff()
	}

	isDefaultSleep := opts.Sleep == 14 || opts.Sleep == 13
	progress, _ := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)

	err := opts.preLoop(progress)
	if err != nil {
		return err
	}

	for {
		e := os.Getenv("TEST_END_SCRAPE")
		ee, _ := strconv.ParseUint(e, 10, 32)
		if ee != 0 && progress.Finalized > ee {
			break
		}

		err := opts.preScrape(progress)
		if err != nil {
			logger.Log(logger.Error, "preScrape", err)
			goto PAUSE
		}

		err = opts.doScrape(progress)
		if err != nil {
			logger.Log(logger.Error, "doScrape:", err)
			goto PAUSE
		}

		// Clean up after this run of the blockScraper
		err = opts.postScrape(progress)
		if err != nil {
			logger.Log(logger.Error, "postScrape:", err)
			goto PAUSE
		}

	PAUSE:
		// TODO: BOGUS - MULTI-CHAIN SPECIFIC
		distanceFromHead := progress.Latest - progress.Staging
		shouldSleep := !isDefaultSleep || distanceFromHead <= (2*opts.UnripeDist)

		sleep := opts.Sleep
		if shouldSleep {
			sleep = utils.Max(sleep, 13)
		}

		if shouldSleep {
			logger.Log(logger.Info, "Sleeping for", sleep, "seconds -", distanceFromHead, "away from head.")
			halfSecs := sleep * 2
			for i := 0; i < int(halfSecs); i++ {
				time.Sleep(time.Duration(500) * time.Millisecond)
			}
		}
	}

	return opts.postLoop(progress)
}

func (opts *ScrapeOptions) doScrape(progressThen *rpcClient.MetaData) error {
	fmt.Println("Calling with", opts.ToCmdLine(), opts.GetEnvStr())
	// TODO: BOGUS - TESTING SCRAPING
	return opts.Globals.PassItOn("blockScrape", opts.Globals.Chain, opts.ToCmdLine(), opts.GetEnvStr())
}
