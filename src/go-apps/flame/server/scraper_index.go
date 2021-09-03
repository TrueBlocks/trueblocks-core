/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

package server

import (
	"fmt"
	"log"
	"os/exec"
	"strconv"

	utils "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/utils"
)

var IndexScraper Scraper

func RunIndexScraper() {
	for true {
		if !IndexScraper.Running {
			if IndexScraper.WasRunning {
				IndexScraper.ShowStateChange("running", "paused")
			}
			IndexScraper.WasRunning = false
			IndexScraper.Pause()
		} else {
			if !IndexScraper.WasRunning {
				IndexScraper.ShowStateChange("paused", "running")
			}
			IndexScraper.WasRunning = true
			IndexScraper.Counter++
			IndexScraper.ShowStateChange("sleep", "wake")
			/*-----------*/
			options := ""
			if IndexScraper.Verbose > 0 {
				options += " --verbose " + strconv.Itoa(int(IndexScraper.Verbose))
			}
			if Options.Pin == true {
				options += " --pins "
			}
			log.Print(IndexScraper.Color, IndexScraper.Name, ": blockScrape", options, utils.Off, "\n")
			cmd := exec.Command("blockScrape", options)
			stderrPipe, err := cmd.StderrPipe()
			if err != nil {
				log.Printf("%s", err)
			} else {
				go func() {
					ScanForProgress(stderrPipe, func(msg string) {
						connectionPool.broadcast <- &Message{
							Action:  ProgressMessage,
							ID:      "block-scrape",
							Content: msg,
						}
					})
				}()
			}
			_, err = cmd.Output()
			if err != nil {
				fmt.Printf("%s", err)
			}
			/*-----------*/
			IndexScraper.ShowStateChange("wake", "sleep")
			if IndexScraper.Running {
				IndexScraper.Pause()
			}
		}
	}
}
