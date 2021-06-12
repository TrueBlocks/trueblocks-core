package scrapers

import (
	"fmt"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
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
			var files []string
			root := "/Users/jrush/Library/Application Support/TrueBlocks/cache/monitors/"
			err := filepath.Walk(root, func(path string, info os.FileInfo, err error) error {
				if strings.Contains(path, ".acct.bin") {
					path = strings.Replace(path, ".acct.bin", "", -1)
					parts := strings.Split(path, "/")
					files = append(files, parts[len(parts)-1])
				}
				return nil
			})
			if err != nil {
				panic(err)
			}
			for _, file := range files {
				if !MonitorScraper.Running {
					break
				}
				options := " --freshen"
				options += " --verbose 10"
				options += " " + file
				log.Println("acctExport", options)
				out, err := exec.Command("acctExport", options).Output()
				if err != nil {
					fmt.Printf("%s", err)
				}
				log.Println("out: ", string(out))
			}
			MonitorScraper.ShowStateChange("wake", "sleep")
			if MonitorScraper.Running {
				MonitorScraper.Pause()
			}
		}
	}
}

/*
	ostringstream os;
    os << "acctExport ";
    os << (opt->cache_txs ? "--cache_txs " : "");
    os << (opt->cache_traces ? "--cache_traces " : "");
    os << (opt->staging ? "--staging " : "");
    os << (opt->unripe ? "--unripe " : "");
    os << (verbose ? ("--verbose " + uint_2_Str(verbose)) : "") << " ";
    os << "--freshen ";
    os << "--first_block " << monitor.getLastBlockInMonitor() << " ";
    os << monitor.address;
    if (opt->load.empty())
        LOG_INFO("Calling: " + substitute(os.str(), "acctExport", "chifra export") + string_q(40, ' '));
    else
        os << "--load " + opt->load;

    if (system(os.str().c_str()) != 0)
        return false;
*/
