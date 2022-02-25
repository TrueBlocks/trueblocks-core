package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"sync"
)

func hasMonitorsFlag(mode string) bool {
	return mode == "monitors" || mode == "both"
}

var MonitorScraper Scraper

func RunMonitorScraper(opts *ScrapeOptions, wg sync.WaitGroup, initialState bool) {
	defer wg.Done()

	var s *Scraper = &MonitorScraper
	s.ChangeState(initialState)

	for {
		if !s.Running {
			if s.WasRunning {
				s.ShowStateChange("running", "paused")
			}
			s.WasRunning = false
			s.Pause()

		} else {
			if !s.WasRunning {
				s.ShowStateChange("paused", "running")
			}
			s.WasRunning = true
			s.Counter++
			s.ShowStateChange("sleep", "wake")

			/* -------------- */
			var addresses []string
			// root := config.Get PathToCache() + "monitors/"
			// err := filepath.Walk(root, func(path string, info os.FileInfo, err error) error {
			// 	if strings.Contains(path, ".acct.bin") {
			// 		path = strings.Replace(path, ".acct.bin", "", -1)
			// 		parts := strings.Split(path, "/")
			// 		addresses = append(addresses, parts[len(parts)-1])
			// 	}
			// 	return nil
			// })
			// if err != nil {
			// 	panic(err)
			// }

			// TODO: Could easily be groups of 5 (or 10 or 20) addresses at a time instead.
			// TODO: It's way faster that way
			for _, addr := range addresses {
				if !MonitorScraper.Running {
					break
				}
				options := " --freshen"
				options += " " + addr
				opts.Globals.PassItOn("acctExport", options)
			}
			/* -------------- */

			s.ShowStateChange("wake", "sleep")
			if s.Running {
				s.Pause()
			}
		}
	}
}
