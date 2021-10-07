package server

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

import (
	"fmt"
	"log"
	"os/exec"
	"strconv"
	"sync"

	utils "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/utils"
)

var IndexScraper Scraper

func RunIndexScraper(wg sync.WaitGroup) {
	IndexScraper.Running = true
	defer wg.Done()
	for {
		fmt.Println("%v", IndexScraper, utils.Off)
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
			cmd := exec.Command(GetCommandPath("blockScrape"), options)
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
