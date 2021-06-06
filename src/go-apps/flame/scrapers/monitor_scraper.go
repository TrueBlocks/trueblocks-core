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
				log.Print(utils.Yellow, "MonitorScraper is paused: ", MonitorScraper.Counter, " ", Counter, utils.Off, "\n")
			}
			MonitorScraper.WasRunning = false
			time.Sleep(1000 * time.Millisecond)
		} else {
			MonitorScraper.WasRunning = true
			MonitorScraper.Counter++
			Counter += 100
			log.Print(utils.Blue, "MonitorScraper awake", utils.Off, "\n")
			var files []string
			root := "/Users/jrush/.quickBlocks/cache/monitors/"
			err := filepath.Walk(root, func(path string, info os.FileInfo, err error) error {
				files = append(files, path)
				return nil
			})
			if err != nil {
				panic(err)
			}
			for _, file := range files {
				fmt.Println(file)
			}
			log.Print(utils.Purple, "MonitorScraper sleeping: ", MonitorScraper.Counter, " ", Counter, utils.Off, "\n")
			time.Sleep(300 * time.Millisecond)
		}
	}
}
