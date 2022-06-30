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

	// progressThen, _ := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)

	var s *scraper.Scraper = &IndexScraper
	s.ChangeState(true)

	for {
		if !s.Running {
			s.Pause()

		} else {
			err := opts.Globals.PassItOn("blockScrape", opts.Globals.Chain, opts.ToCmdLine(), opts.GetEnvStr())
			if err != nil {
				fmt.Println("blockScrape returned an error:", err)
			} else {
				// TODO: BOGUS - MANIFEST WRITING THE MANIFEST
				err = opts.publishManifest()
				if err != nil {
					fmt.Println("publishManifest returned an error:", err)
				}
				// TODO: BOGUS -- PIN THE FILE TO PINATA -- REPORT THE IPFS HASH
				// TODO: BOGUS - MANIFEST WRITING THE MANIFEST AND PINNING
				// progressNow, _ := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
				// // fmt.Println(colors.BrightBlue, progressThen.Finalized, colors.BrightWhite, progressNow.Finalized, colors.Off, progressNow.Finalized > progressThen.Finalized)
				// if progressNow.Finalized > progressThen.Finalized {
				// 	fmt.Println(colors.Yellow, "Need to pin here if enabled", colors.Off)
				// 	// time.Sleep(time.Second * 3)
				// 	err = opts.publishManifest()
				// 	if err != nil {
				// 		fmt.Println("publishManifest returned an error:", err)
				// 	}
				// 	// } else {
				// 	// 	fmt.Println(colors.Cyan, "Do not need to pin here", colors.Off)
				// 	// 	time.Sleep(time.Second * 3)
				// }
				// progressThen = progressNow
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

// TODO: BOGUS - MANIFEST WRITING THE MANIFEST
func (opts *ScrapeOptions) publishManifest() error {
	cacheManifest, err := manifest.ReadManifest(opts.Globals.Chain, manifest.FromCache)
	if err != nil {
		return err
	}
	// TODO: BOGUS DOES THIS DESTROY THE FILE ON DISC BEFORE WRITING TO IT? I THINK IT DOES.
	fileName := config.GetPathToChainConfig(opts.Globals.Chain) + "manifest.json"
	w, err := os.Create(fileName)
	if err != nil {
		return fmt.Errorf("creating file: %s", err)
	}
	defer w.Close()
	err = file.Lock(w)
	if err != nil {
		return fmt.Errorf("locking file: %s", err)
	}
	return opts.Globals.RenderManifest(w, "json", cacheManifest)
}
