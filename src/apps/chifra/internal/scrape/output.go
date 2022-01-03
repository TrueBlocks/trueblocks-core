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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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
	IndexScraper = NewScraper(colors.Yellow, "IndexScraper", Options.Sleep)
	go RunIndexScraper(wg, hasIndexerFlag(args[0]))

	wg.Add(1)
	MonitorScraper = NewScraper(colors.Purple, "MonitorScraper", Options.Sleep)
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
			Options.Globals.PassItOn("blockScrape", Options.ToCmdLine())
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
			// root := config.GetPathToCache() + "monitors/"
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
				Options.Globals.PassItOn("acctExport", options)
			}
			/* -------------- */

			s.ShowStateChange("wake", "sleep")
			if s.Running {
				s.Pause()
			}
		}
	}
}

var cachePath string = "/tmp/"

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
	scraper.Verbose = int64(Options.Globals.LogLevel)
	return *scraper
}

func (scraper *Scraper) ShowStateChange(from, to string) {
	logger.Log(logger.Info, scraper.Color, scraper.Name, ": [", from, " --> ", to, "]", colors.Off)
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
	fileName := cachePath + scraper.Name + ".txt"
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

// // ChifraStatus
// type ChifraStatus struct {
// 	Client     string `json:"clientVersion"`
// 	TrueBlocks string `json:"trueblocksVersion"`
// 	RPC        string `json:"rpcProvider"`
// 	ConfigPath string `json:"configPath"`
// 	Cache Path  string `json:"cachePath"`
// 	IndexPath  string `json:"indexPath"`
// 	IsTesting  bool   `json:"isTesting"`
// 	IsDocker   bool   `json:"isDocker"`
// 	IsArchive  bool   `json:"isArchive"`
// 	IsTracing  bool   `json:"isTracing"`
// 	HasEsKey   bool   `json:"hasEskey"`
// 	HasPinKey  bool   `json:"hasPinkey"`
// 	Date       string `json:"date"`
// }

// // ChifraMeta
// type ChifraMeta struct {
// 	Client    int64
// 	Finalized int64
// 	Staging   int64
// 	Ripe      int64
// 	Unripe    int64
// }

// // ChifraResponse is sent by the command line chifra
// type ChifraResponse struct {
// 	Status []ChifraStatus `json:"data"`
// 	Meta   ChifraMeta     `json:"meta"`
// }

// func GetChifraResponse() (ChifraResponse, error) {
// 	cmd := exec.Command(config.Get PathToCommands("cacheStatus"), "--terse")
// 	cmd.Env = append(os.Environ(), "API_MODE=true")
// 	out, err := cmd.Out put()
// 	if err != nil {
// 		fmt.Printf("ChifraResponse:cmd.Out put() failed: %s", err)
// 		var junk ChifraResponse
// 		return junk, err
// 	}
// 	var response ChifraResponse
// 	err = json.Unmarshal(out, &response)
// 	if err != nil {
// 		fmt.Printf("ChifraResponse:json.Unmarshal() failed: %s", err)
// 		var junk ChifraResponse
// 		return junk, err
// 	}
// 	return response, nil
// }

// func GetChifraData() (ChifraStatus, error) {
// 	response, err := GetChifraResponse()
// 	if err != nil {
// 		var junk ChifraStatus
// 		return junk, err
// 	}
// 	if len(response.Status) == 0 {
// 		fmt.Printf("GetChifraData.len(response.Status) == 0")
// 		var junk ChifraStatus
// 		return junk, err
// 	}
// 	return response.Status[0], nil
// }

// func GetChifraMeta() (ChifraMeta, error) {
// 	response, err := GetChifraResponse()
// 	if err != nil {
// 		fmt.Printf("GetChifraMeta.GetChifraResponse failed with: %s", err)
// 		var junk ChifraMeta
// 		return junk, err
// 	}
// 	return response.Meta, nil
// }

// func ParseOptions() error {
// 		// Establish and parse the command line input...
// 		flag.BoolVar(&Options.Scrape, "scrape", false, "enable block scraper mode")

// 		flag.BoolVar(&Options.Monitor, "monitor", false, "enable monitor scraper mode")

// 		// flag.IntVar(&Options.Verbose, "verbose", 0, "verbose level (between 0 and 10 inclusive)")
// 		flag.BoolVar(&Options.Pin, "pin", false, "pins Bloom filters and chunks to pinning service (requires API key)")
// 		flag.BoolVar(&Options.Publish, "publish", false, "whatever")
// 		flag.Uint64Var(&Options.Block_Cnt, "block_cnt", 2000, "number of blocks to process at a time")
// 		flag.Float64Var(&Options.Sleep, "sleep", 14., "specifies sleep interval between scrapes")

// 		flag.Parse()

// Options.Status, _ = GetChifraData()
// Options.Meta, _ = GetChifraMeta()

// 	if Options.Verbose > 0 {
// 		log.Print(colors.Green, "verbose:     ", colors.Off, Options.Verbose, "\n")
// 	}

// 	log.Print("\n")
// 	log.Print(colors.Green, "Client:       ", colors.Off, Options.Status.Latest)
// 	log.Print(colors.Green, "TrueBlocks:   ", colors.Off, Options.Status.TrueBlocks)
// 	log.Print(colors.Green, "Cache Path:   ", colors.Off, config.GetPathToCache())
// 	log.Print(colors.Green, "Index Path:   ", colors.Off, config.GetPathToIndex())
// 	log.Print(colors.Green, "Rpc Provider: ", colors.Off, Options.Status.RPC)
// 	log.Print(colors.Green, "Progress:     ", colors.Off, Options.Meta.Latest, ", ", Options.Meta.Finalized, ", ", Options.Meta.Staging, ", ", Options.Meta.Unripe)

// 	return nil
// }

// // ManageScraper handles scraper commands
// func ManageScraper(w http.ResponseWriter, r *http.Request) {
// 	status, exists := utils.GetParam("status", "both", r)
// 	if !exists {
// 		toggle, ok := utils.GetParam("toggle", "both", r)
// 		if !ok {
// 			log.Println("'toggle' param is missing")
// 			return
// 		}
// 		mode, ok := utils.GetParam("mode", "", r)
// 		if !ok {
// 			log.Println("'mode' param is missing")
// 			return
// 		}
// 		if hasIndexerFlag(toggle) {
// 			IndexScraper.ChangeState(mode == "true")
// 		}
// 		if hasMonitorsFlag(toggle) {
// 			MonitorScraper.ChangeState(mode == "true")
// 		}
// 		scraperStatus("toggle", w, r)
// 	} else {
// 		log.Println("status: ", status)
// 		scraperStatus("status", w, r)
// 	}
// }

// // scraperStatus presents the result
// func scraperStatus(msg string, w http.ResponseWriter, r *http.Request) {
// 	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
// 	w.Header().Set("Access-Control-Allow-Origin", "*")
// 	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
// 	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
// 	w.WriteHeader(http.StatusOK)
// 	fmt.Fprint(w, "{ ")
// 	fmt.Fprint(w, "\"msg\": \"")
// 	fmt.Fprint(w, msg)
// 	fmt.Fprint(w, "\", ")
// 	fmt.Fprint(w, "\"indexer\": ")
// 	fmt.Fprint(w, IndexScraper.ToJson())
// 	fmt.Fprint(w, ", ")
// 	fmt.Fprint(w, "\"monitor\": ")
// 	fmt.Fprint(w, MonitorScraper.ToJson())
// 	fmt.Fprint(w, " }")
// }
// EXISTING_CODE
