package scrapers

import (
	"fmt"
	"log"
	"os"
	"path/filepath"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/utils"
)

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

var MonitorScraper Scraper

func RunMonitorScraper() {
	for true {
		if !MonitorScraper.Running {
			if MonitorScraper.WasRunning {
				MonitorScraper.ChangeState("running", "paused")
			}
			MonitorScraper.WasRunning = false
			time.Sleep(time.Duration(MonitorScraper.Sleep) * time.Millisecond)
		} else {
			MonitorScraper.WasRunning = true
			MonitorScraper.Counter++
			log.Print(utils.Purple, "MonitorScraper ", utils.Blue, "[sleep --> wake]", utils.Off, ": ", MonitorScraper.Counter, utils.Off, "\n")
			var files []string
			root := "/Users/jrush/Library/Application Support/TrueBlocks/cache/monitors/"
			err := filepath.Walk(root, func(path string, info os.FileInfo, err error) error {
				files = append(files, path)
				return nil
			})
			if err != nil {
				panic(err)
			}
			for _, file := range files {
				fmt.Println(file)
				time.Sleep(500 * time.Millisecond)
				if !MonitorScraper.Running {
					break
				}
			}
			log.Print(utils.Purple, "MonitorScraper ", utils.Blue, "[wake --> sleep]", utils.Off, ": ", MonitorScraper.Counter, utils.Off, "\n")
			time.Sleep(time.Duration(MonitorScraper.Sleep) * time.Millisecond)
		}
	}
}
