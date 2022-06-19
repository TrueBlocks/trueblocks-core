package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"log"
	"os"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/scraper"
)

var IndexScraper scraper.Scraper

func (opts *ScrapeOptions) RunIndexScraper(wg *sync.WaitGroup) {
	defer wg.Done()

	var s *scraper.Scraper = &IndexScraper
	s.ChangeState(true)

	for {
		if !s.Running {
			s.Pause()

		} else {
			err := opts.Globals.PassItOn("blockScrape", opts.Globals.Chain, opts.ToCmdLine(), opts.GetEnvStr())
			if err != nil {
				fmt.Println("Error returned from blockScape:", err)
			}
			err = opts.publishManifast()
			if err != nil {
				fmt.Println("Error returned from publishManifast:", err)
			}

			if s.Running {
				// We sleep under two conditions
				//   1) the user has told us an explicit amount of time to Sleep
				//   2) we're close enough to the head that we should sleep because there
				//      are no new blocks (UnripeDist defaults to 28 blocks)
				//
				// If we're closeEnough and the user specified a sleep value less than
				// 14 seconds, there's not reason to not sleep
				// TODO: Multi-chain specific
				var distanceFromHead uint64 = 13
				meta, err := rpcClient.GetMetaData(opts.Globals.Chain, false)
				if err != nil {
					log.Println("Error from node:", err)
				} else {
					distanceFromHead = meta.Latest - meta.Staging
				}
				closeEnough := distanceFromHead <= (2 * opts.UnripeDist)
				// TODO: Multi-chain specific
				if closeEnough && opts.Sleep < 13 {
					// TODO: Multi-chain specific
					opts.Sleep = 13
				}
				// TODO: Multi-chain specific
				isDefault := opts.Sleep == 14 || opts.Sleep == 13
				if !isDefault || closeEnough {
					if closeEnough {
						logger.Log(logger.Info, "Close enough to head. Sleeping for", opts.Sleep, "seconds -", distanceFromHead, "away from head.")
					} else {
						logger.Log(logger.Info, "Sleeping for", opts.Sleep, "seconds -", distanceFromHead, "away from head.")
					}
					s.Pause()
				}
			}
		}
	}
}

func (opts *ScrapeOptions) publishManifast() error {
	manFromCache, err := manifest.FromCache(opts.Globals.Chain)
	if err != nil {
		return err
	}
	fileName := config.GetPathToChainConfig(opts.Globals.Chain) + "manifest.json"
	// TODO: See SaveManifest - consolidate it
	w, err := os.Create(fileName)
	if err != nil {
		return fmt.Errorf("creating file: %s", err)
	}
	defer w.Close()
	err = file.Lock(w)
	if err != nil {
		return fmt.Errorf("locking file: %s", err)
	}
	return opts.Globals.RenderManifest(w, "json", manFromCache)
}
