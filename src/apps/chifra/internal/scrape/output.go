// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package scrapePkg

// EXISTING_CODE
import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"sync"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	// "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunScrape(cmd *cobra.Command, args []string) error {
	opts := ScrapeFinishParse(args)

	err := opts.ValidateScrape()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	var wg sync.WaitGroup

	wg.Add(1)
	IndexScraper = NewScraper(colors.Yellow, "IndexScraper", opts.Sleep)
	go RunIndexScraper(wg, hasIndexerFlag(args[0]))

	wg.Add(1)
	MonitorScraper = NewScraper(colors.Purple, "MonitorScraper", opts.Sleep)
	go RunMonitorScraper(wg, hasMonitorsFlag(args[0]))

	wg.Wait()

	return nil
	// EXISTING_CODE
}

func ServeScrape(w http.ResponseWriter, r *http.Request) bool {
	opts := FromRequest(w, r)

	err := opts.ValidateScrape()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
func hasIndexerFlag(mode string) bool {
	return mode == "indexer" || mode == "both"
}

func hasMonitorsFlag(mode string) bool {
	return mode == "monitors" || mode == "both"
}

var IndexScraper Scraper
var MonitorScraper Scraper

func RunIndexScraper(wg sync.WaitGroup, initialState bool) {
	var s *Scraper = &IndexScraper

	defer wg.Done()

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
			GetOptions().Globals.PassItOn("blockScrape", GetOptions().ToCmdLine())
			/* -------------- */

			s.ShowStateChange("wake", "sleep")
			if s.Running {
				s.Pause()
			}
		}
	}
}

func RunMonitorScraper(wg sync.WaitGroup, initialState bool) {
	var s *Scraper = &MonitorScraper

	defer wg.Done()

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

			// TODO: Could easily be groups of 5 (or 10 or 20) addresses at a time instead. Way faster
			for _, addr := range addresses {
				if !MonitorScraper.Running {
					break
				}
				options := " --freshen"
				options += " " + addr
				GetOptions().Globals.PassItOn("acctExport", options)
			}
			/* -------------- */

			s.ShowStateChange("wake", "sleep")
			if s.Running {
				s.Pause()
			}
		}
	}
}

var statusPath string = "/tmp/"

type Scraper struct {
	Counter    uint64 `json:"Counter"`
	Running    bool   `json:"Running"`
	WasRunning bool   `json:""`
	SleepSecs  int64  `json:"SleepSecs"`
	Color      string `json:"Color"`
	Name       string `json:"Name"`
	Verbose    int64  `json:"Verbose"`
}

func NewScraper(color, name string, secs float64) Scraper {
	scraper := new(Scraper)
	scraper.Color = color
	scraper.Name = name
	scraper.SleepSecs = int64(secs)
	scraper.Running = false
	scraper.Verbose = int64(GetOptions().Globals.LogLevel)
	return *scraper
}

func (scraper *Scraper) ShowStateChange(from, to string) {
	// logger.Log(logger.Info, scraper.Color, scraper.Name, ": [", from, " --> ", to, "]", colors.Off)
}

func (scraper *Scraper) ToJson() string {
	e, err := json.Marshal(scraper)
	if err != nil {
		fmt.Printf("%s", err)
		return ""
	}
	return string(e)
}

func (scraper *Scraper) ChangeState(onOff bool) bool {
	prev := scraper.Running
	scraper.Running = onOff
	str := "false"
	if onOff {
		str = "true"
	}
	fileName := statusPath + scraper.Name + ".txt"
	err := ioutil.WriteFile(fileName, []byte(str), 0644)
	if err != nil {
		log.Fatal(err)
	}
	return prev
}

func (scraper *Scraper) Pause() {
	halfSecs := scraper.SleepSecs * 2
	state := scraper.Running
	for i := 0; i < int(halfSecs); i++ {
		if state != scraper.Running {
			break
		}
		time.Sleep(time.Duration(500) * time.Millisecond)
	}
}

// EXISTING_CODE
