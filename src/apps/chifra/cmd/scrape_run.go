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
package cmd

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"os/exec"
	"sync"

	// "time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

// TODO: this is a much more elegant way to do error strings:
// TODO: https://github.com/storj/uplink/blob/v1.7.0/bucket.go#L19

func validateScrapeArgs(cmd *cobra.Command, args []string) error {
	if len(args) == 0 {
		return validate.Usage("Please choose one of [indexer|monitors|both]")

	} else {
		for _, arg := range args {
			err := validate.ValidateEnum("mode", arg, "[indexer|monitors|both]")
			if err != nil {
				return err
			}
		}
	}

	if ScrapeOpts.Action == "" {
		ScrapeOpts.Action = "run"
	}
	err := validate.ValidateEnum("action", ScrapeOpts.Action, "[toggle|run|restart|pause|quit]")
	if err != nil {
		return err
	}

	if ScrapeOpts.Sleep < .5 {
		return validate.Usage("Values less that .5 seconds for --sleep are not allowed.")
	}

	if ScrapeOpts.Pin && !hasIndexerFlag(args[0]) {
		return validate.Usage("The --pin option is only available with the indexer.")
	}

	if ScrapeOpts.Publish && !hasIndexerFlag(args[0]) {
		return validate.Usage("The --publish option only works with the indexer.")
	}

	err = validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	return nil
}

func runScrape(cmd *cobra.Command, args []string) {
	IndexScraper = NewScraper(colors.Yellow, "IndexScraper", ScrapeOpts.Sleep, 0) // ScrapeOpts.Verbose)

	MonitorScraper = NewScraper(colors.Purple, "MonitorScraper", ScrapeOpts.Sleep, 0) // ScrapeOpts.Verbose)

	var wg sync.WaitGroup

	wg.Add(1)
	go RunIndexScraper(wg)
	if hasIndexerFlag(args[0]) {
		IndexScraper.ChangeState(true)
	}

	wg.Add(1)
	go RunMonitorScraper(wg)
	if hasMonitorsFlag(args[0]) {
		MonitorScraper.ChangeState(true)
	}

	wg.Wait()
}

var IndexScraper Scraper

func RunIndexScraper(wg sync.WaitGroup) {
	IndexScraper.Running = true
	defer wg.Done()
	for {
		// fmt.Printf("%s\n", IndexScraper.ToJson())
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
			if ScrapeOpts.Pin {
				options += " --pin "
			}
			if ScrapeOpts.Publish {
				options += " --publish "
			}
			if ScrapeOpts.Sleep != 14. {
				options += " --sleep " + fmt.Sprintf("%g", ScrapeOpts.Sleep)
			}
			options += (" --block_cnt " + fmt.Sprintf("%d", ScrapeOpts.Block_Cnt))
			PassItOn("blockScrape", options, "")
			/*-----------*/
			IndexScraper.ShowStateChange("wake", "sleep")
			if IndexScraper.Running {
				IndexScraper.Pause()
			}
		}
	}
}

// // ChifraStatus
// type ChifraStatus struct {
// 	Client     string `json:"client_version"`
// 	TrueBlocks string `json:"trueblocks_version"`
// 	RPC        string `json:"rpc_provider"`
// 	ConfigPath string `json:"config_path"`
// 	CachePath  string `json:"cache_path"`
// 	IndexPath  string `json:"index_path"`
// 	IsTesting  bool   `json:"is_testing"`
// 	IsDocker   bool   `json:"is_docker"`
// 	IsArchive  bool   `json:"is_archive"`
// 	IsTracing  bool   `json:"is_tracing"`
// 	HasEsKey   bool   `json:"has_eskey"`
// 	HasPinKey  bool   `json:"has_pinkey"`
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
// 	cmd := exec.Command(utils.GetCommandPath("cacheStatus"), "--terse")
// 	cmd.Env = append(os.Environ(), "API_MODE=true")
// 	out, err := cmd.Output()
// 	if err != nil {
// 		fmt.Printf("ChifraResponse:cmd.Output() failed: %s", err)
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
// 		flag.BoolVar(&ScrapeOpts.Scrape, "scrape", false, "enable block scraper mode")

// 		flag.BoolVar(&ScrapeOpts.Monitor, "monitor", false, "enable monitor scraper mode")

// 		// flag.IntVar(&ScrapeOpts.Verbose, "verbose", 0, "verbose level (between 0 and 10 inclusive)")
// 		flag.BoolVar(&ScrapeOpts.Pin, "pin", false, "pins Bloom filters and chunks to pinning service (requires API key)")
// 		flag.BoolVar(&ScrapeOpts.Publish, "publish", false, "whatever")
// 		flag.Uint64Var(&ScrapeOpts.Block_Cnt, "block_cnt", 2000, "number of blocks to process at a time")
// 		flag.Float64Var(&ScrapeOpts.Sleep, "sleep", 14., "specifies sleep interval between scrapes")

// 		flag.Parse()

// 		// ScrapeOpts.Status, _ = GetChifraData()
// 		// ScrapeOpts.Meta, _ = GetChifraMeta()

// 	IndexScraper = NewScraper(Yellow, "IndexScraper", ScrapeOpts.Sleep, 0) // ScrapeOpts.Verbose)
// 	if ScrapeOpts.Scrape {
// 		log.Print(colors.Green, "scraping:    ", colors.Off, ScrapeOpts.Scrape, "\n")
// 		if ScrapeOpts.Sleep != 14. {
// 			log.Print(colors.Green, "sleep:    ", colors.Off, ScrapeOpts.Sleep, "\n")
// 		}
// 		IndexScraper.ChangeState(true)
// 	}

// 	MonitorScraper = NewScraper(Purple, "MonitorScraper", ScrapeOpts.Sleep, 0) // ScrapeOpts.Verbose)
// 	if ScrapeOpts.Monitor {
// 		log.Print(colors.Green, "monitoring:  ", colors.Off, ScrapeOpts.Monitor, "\n")
// 		MonitorScraper.ChangeState(true)
// 	}

// 	if ScrapeOpts.Verbose > 0 {
// 		log.Print(colors.Green, "verbose:     ", colors.Off, ScrapeOpts.Verbose, "\n")
// 	}

// 	log.Print("\n")
// 	log.Print(colors.Green, "Client:       ", colors.Off, ScrapeOpts.Status.Client)
// 	log.Print(colors.Green, "TrueBlocks:   ", colors.Off, ScrapeOpts.Status.TrueBlocks)
// 	log.Print(colors.Green, "Cache Path:   ", colors.Off, ScrapeOpts.Status.CachePath)
// 	log.Print(colors.Green, "Index Path:   ", colors.Off, ScrapeOpts.Status.IndexPath)
// 	log.Print(colors.Green, "Rpc Provider: ", colors.Off, ScrapeOpts.Status.RPC)
// 	log.Print(colors.Green, "Progress:     ", colors.Off, ScrapeOpts.Meta.Client, ", ", ScrapeOpts.Meta.Finalized, ", ", ScrapeOpts.Meta.Staging, ", ", ScrapeOpts.Meta.Unripe)

// 	return nil
// }

var MonitorScraper Scraper

func RunMonitorScraper(wg sync.WaitGroup) {
	defer wg.Done()
	for {
		if !MonitorScraper.Running {
			if MonitorScraper.WasRunning {
				MonitorScraper.ShowStateChange("running", "paused")
			}
			MonitorScraper.WasRunning = false
			MonitorScraper.Pause()
		} else {
			if !MonitorScraper.WasRunning {
				MonitorScraper.ShowStateChange("paused", "running")
			}
			MonitorScraper.WasRunning = true
			MonitorScraper.Counter++
			MonitorScraper.ShowStateChange("sleep", "wake")
			var addresses []string
			// root := ScrapeOpts.Status.CachePath + "monitors/"
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

			for index, addr := range addresses {
				if !MonitorScraper.Running {
					break
				}
				options := " --freshen"
				// if MonitorScraper.Verbose > 0 {
				//	options += " --verbose " + strconv.Itoa(int(MonitorScraper.Verbose))
				// }
				options += " " + addr
				log.Print("<PROG> : ", MonitorScraper.Color, "Freshening ", index, " of ", len(addresses), " acctExport", options, colors.Off, "\n")
				cmd := exec.Command(utils.GetCommandPath("acctExport"), options)
				stderrPipe, err := cmd.StderrPipe()
				if err != nil {
					log.Printf("%s", err)
				} else {
					go func() {
						ScanForProgress(stderrPipe, func(msg string) {
							if len(msg) > 0 {
								log.Printf("%s", msg)
							}
						})
					}()
				}
				_, err = cmd.Output()
				if err != nil {
					fmt.Printf("%s", err)
				}
			}

			MonitorScraper.ShowStateChange("wake", "sleep")
			if MonitorScraper.Running {
				MonitorScraper.Pause()
			}
		}
	}
}

type Scraper struct {
	Counter    uint64 `json:"Counter"`
	Running    bool   `json:"Running"`
	WasRunning bool   `json:""`
	SleepSecs  int64  `json:"SleepSecs"`
	Color      string `json:"Color"`
	Name       string `json:"Name"`
	Verbose    int64  `json:"Verbose"`
}

func NewScraper(color, name string, secs float64, verbose int) Scraper {
	scraper := new(Scraper)
	scraper.Color = color
	scraper.Name = name
	scraper.SleepSecs = int64(secs)
	scraper.Running = scraper.LoadStateFromCache()
	scraper.Verbose = int64(verbose)
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

var cachePath string = "/Users/jrush/Library/Application Support/Trueblocks/cache/"

func (scraper *Scraper) ChangeState(onOff bool) bool {
	prev := scraper.Running
	scraper.Running = onOff
	str := "false"
	if onOff {
		str = "true"
	}
	fileName := cachePath + "tmp/" + scraper.Name + ".txt"
	err := ioutil.WriteFile(fileName, []byte(str), 0644)
	if err != nil {
		log.Fatal(err)
	}
	return prev
}

func (scraper *Scraper) LoadStateFromCache() bool {
	fileName := cachePath + "tmp/" + scraper.Name + ".txt"
	if !utils.FileExists(fileName) {
		return false
	}
	content, err := ioutil.ReadFile(fileName)
	if err != nil {
		log.Fatal(err)
	}
	return string(content) == "true"
}

func (scraper *Scraper) Pause() {
	// halfSecs := scraper.SleepSecs * 2
	// state := scraper.Running
	// for i := 0; i < int(halfSecs); i++ {
	// 	if state != scraper.Running {
	// 		break
	// 	}
	// 	time.Sleep(time.Duration(500) * time.Millisecond)
	// }
}

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

// 		if toggle == "indexer" || toggle == "both" {
// 			IndexScraper.ChangeState(mode == "true")
// 		}
// 		if toggle == "monitors" || toggle == "both" {
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

func hasIndexerFlag(mode string) bool {
	return mode == "indexer" || mode == "both"
}

func hasMonitorsFlag(mode string) bool {
	return mode == "monitors" || mode == "both"
}
