// TODO: BOGUS - MULTI-CHAIN SPECIFIC SLEEPING IS
// TODO: BOGUS - TESTING SCRAPING
// TODO: BOGUS - TESTING SCRAPING
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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

func (opts *ScrapeOptions) HandleScrape() error {
	progress, _ := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)

	ok, err := opts.preLoop(progress)
	if !ok || err != nil {
		return err
	}

	for {
		ok, err := opts.preScrape(progress)
		if !ok || err != nil {
			if !ok {
				break
			}
			logger.Log(logger.Error, "preScrape", err)
			goto PAUSE
		}

		fmt.Println("Calling with", opts.toCmdLine(), opts.getEnvStr())
		err = opts.Globals.PassItOn("blockScrape", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStr())
		if err != nil {
			logger.Log(logger.Error, "blockScrape", err)
			goto PAUSE
		}

		// Clean up after this run of the blockScraper
		ok, err = opts.postScrape(progress)
		if !ok || err != nil {
			if !ok {
				break
			}
			logger.Log(logger.Error, "postScrape", err)
			goto PAUSE
		}

	PAUSE:
		opts.pause(progress)
	}

	_, err = opts.postLoop(progress)
	return err
}
