package scrapePkg

// TODO: BOGUS - MULTI-CHAIN SPECIFIC SLEEPING IS
// TODO: BOGUS - THINGS TO DO:
// TODO: BOGUS - CLEANUP BETWEEN RUNS IF THINGS DON'T WORK OUT AND SOMETHING HAD TO QUIT EARLY
// TODO: BOGUS - MAKE SURE WE'RE NOT RUNNING IF ACCTSCRAPE (OR ANYTHING ELSE?) IS RUNNINGs
// TODO: BOGUS - RESPOND WELL TO RUNNING AGAINST NON-ARCHIVE NODES see isArchiveNode() in C++ code
// TODO: BOGUS - RETRY ON FAILURE OF BLAZE - SEE NOTES BELOW
// TODO: BOGUS - THINGS TO DO:
// TODO: BOGUS - CLEANUP BETWEEN RUNS IF THINGS DON'T WORK OUT AND SOMETHING HAD TO QUIT EARLY
// TODO: BOGUS - MAKE SURE WE'RE NOT RUNNING IF ACCTSCRAPE (OR ANYTHING ELSE?) IS RUNNINGs
// TODO: BOGUS - RESPOND WELL TO RUNNING AGAINST NON-ARCHIVE NODES see isArchiveNode() in C++ code

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

func (opts *ScrapeOptions) HandleScrape() error {
	progress, err := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	if err != nil {
		return err
	}

	if ok, err := opts.ScrapePreLoop(progress); !ok || err != nil {
		return err
	}

	for {
		if ok, err := opts.ScrapePreScrape(progress); !ok || err != nil {
			if !ok {
				break
			}
			logger.Log(logger.Error, "ScrapePreScrape", err)
			goto PAUSE
		}

		if ok, err := opts.ScrapeScrape(progress); !ok || err != nil {
			if !ok {
				break
			}
			logger.Log(logger.Error, "blockScrape", err)
			goto PAUSE
		}

		// Clean up after this run of the blockScraper
		if ok, err := opts.ScrapePostScrape(progress); !ok || err != nil {
			if !ok {
				break
			}
			logger.Log(logger.Error, "postScrape", err)
			goto PAUSE
		}

	PAUSE:
		opts.Z_6_pause(progress)
	}

	_, err = opts.ScrapePostLoop(progress)
	return err
}

// TODO: BOGUS - NOTES ON RE-RUN AFTER BLAZE FAILURE
// cleanFolder(indexFolder_ripe);
// static bool failed_already = false;
// if (!failed_already) {
// 	failed_already = true;
// 	LOG_WARN(cYellow, "Blaze quit without finishing. Retrying...", cOff);
// 	sleep(3);
// 	return scrape_blocks();
// }
// LOG_WARN(cYellow, "Blaze quit without finishing twice. Reprocessing...", cOff);
