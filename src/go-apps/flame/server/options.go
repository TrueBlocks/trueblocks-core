package server

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

import (
	"fmt"
	"log"
	"os/exec"
	"strings"

	scrapers "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/scrapers"
	utils "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/utils"
	flag "github.com/spf13/pflag"
)

// globalOptionsT makes command line options available to the package
type globalOptionsT struct {
	Scrape      bool
	Monitor     bool
	Port        string
	Client      string
	Version     string
	ConfigPath  string
	CachePath   string
	IndexPath   string
	RpcProvider string
	Verbose     int
}

// Options carries command line options
var Options globalOptionsT

func ParseOptions() error {
	// Establish and parse the command line input...
	flag.BoolVar(&Options.Scrape, "scrape", false, "enable block scraper mode")

	flag.BoolVar(&Options.Monitor, "monitor", false, "enable monitor scraper mode")

	flag.StringVar(&Options.Port, "port", ":8080", "specify the server's port")
	if !strings.HasPrefix(Options.Port, ":") {
		Options.Port = ":" + Options.Port
	}

	flag.IntVar(&Options.Verbose, "verbose", 0, "verbose level (between 0 and 10 inclusive)")
	flag.Lookup("verbose").NoOptDefVal = "0"

	flag.Parse()

	scrapers.IndexScraper = scrapers.NewScraper(utils.Yellow, "IndexScraper", 14)
	if Options.Scrape {
		log.Print(utils.Green, "scraping:    ", utils.Off, Options.Scrape, "\n")
		scrapers.IndexScraper.ChangeState(true)
	}

	scrapers.MonitorScraper = scrapers.NewScraper(utils.Purple, "MonitorScraper", 14)
	if Options.Monitor {
		log.Print(utils.Green, "monitoring:  ", utils.Off, Options.Monitor, "\n")
		scrapers.MonitorScraper.ChangeState(true)
	}

	if Options.Port != ":8080" {
		log.Print(utils.Green, "port:        ", utils.Off, Options.Port, "\n")
	} else {
		Options.Port = "127.0.0.1:8080"
	}

	if Options.Verbose > 0 {
		log.Print(utils.Green, "verbose:     ", utils.Off, Options.Verbose, "\n")
	}

	out, err := exec.Command("cacheStatus", "--terse").Output()
	if err != nil {
		fmt.Printf("%s", err)
	}

	fmt.Println("")
	log.Print(utils.Green, "Starting TrueBlocks API server on port "+Options.Port, utils.Off, "\n")
	str := string(out[:])
	lines := strings.Split(str, "\n")
	for _, line := range lines {
		if line != "" {
			if strings.HasPrefix(line, "client") {
				parts := strings.Split(line, " ")
				log.Print(utils.Green, parts[0], " ", utils.Off, parts[1], "\n")
			} else {
				parts := strings.Split(line, " ")
				log.Print(utils.Green, parts[2], " ", utils.Off, parts[3], "\n")
			}
		}
	}

	return nil
}
