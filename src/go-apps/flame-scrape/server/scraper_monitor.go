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
	"os"
	"os/exec"
	"path/filepath"
	"strconv"
	"strings"
	"sync"

	utils "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/utils"
)

var MonitorScraper Scraper

func RunMonitorScraper(wg sync.WaitGroup) {
	defer wg.Done()
	for {
		fmt.Println("%v", MonitorScraper, utils.Off)
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
			root := Options.Status.CachePath + "monitors/"
			err := filepath.Walk(root, func(path string, info os.FileInfo, err error) error {
				if strings.Contains(path, ".acct.bin") {
					path = strings.Replace(path, ".acct.bin", "", -1)
					parts := strings.Split(path, "/")
					addresses = append(addresses, parts[len(parts)-1])
				}
				return nil
			})
			if err != nil {
				panic(err)
			}

			for index, addr := range addresses {
				if !MonitorScraper.Running {
					break
				}
				options := " --freshen"
				if MonitorScraper.Verbose > 0 {
					options += " --verbose " + strconv.Itoa(int(MonitorScraper.Verbose))
				}
				options += " " + addr
				log.Print("<PROG> : ", MonitorScraper.Color, "Freshening ", index, " of ", len(addresses), " acctExport", options, utils.Off, "\n")
				cmd := exec.Command(GetCommandPath("acctExport"), options)
				stderrPipe, err := cmd.StderrPipe()
				if err != nil {
					log.Printf("%s", err)
				} else {
					go func() {
						ScanForProgress(stderrPipe, func(msg string) {
							connectionPool.broadcast <- &Message{
								Action:  ProgressMessage,
								ID:      "monitor-scrape",
								Content: msg,
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

/*
	ostringstream os;
    os << "acctExport ";
    os << (opt->cache_txs ? "--cache_txs " : "");
    os << (opt->cache_traces ? "--cache_traces " : "");
    os << (opt->staging ? "--staging " : "");
    os << (opt->unripe ? "--unripe " : "");
    os << (verbose ? ("--verbose " + uint_2_Str(verbose)) : "") << " ";
    os << "--freshen ";
    os << "--first_block " << monitor.getLastBlockInMonitorPlusOne() << " ";
    os << monitor.address;
    if (opt->load.empty())
        LOG_INFO("Calling: " + substitute(os.str(), "acctExport", "chifra export") + string_q(40, ' '));
    else
        os << "--load " + opt->load;

    if (system(os.str().c_str()) != 0)
        return false;
*/
