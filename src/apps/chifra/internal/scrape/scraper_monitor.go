package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"log"
	"os"
	"path/filepath"
	"strings"
	"sync"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

func hasMonitorsFlag(mode string) bool {
	return mode == "monitors" || mode == "both"
}

var MonitorScraper Scraper

// RunMonitorScraper runs continually, never stopping and freshens any existing monitors
func RunMonitorScraper(opts *ScrapeOptions, wg sync.WaitGroup, initialState bool) {
	defer wg.Done()

	chain := opts.Globals.Chain
	prevChunk := uint64(0)
	first := true

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
			thisChunk := rpcClient.GetMetaData(chain, false).Finalized
			fmt.Println("first: ", first, " prevChunk: ", prevChunk, " thisChunk: ", thisChunk)
			if !first && prevChunk == thisChunk {
				fmt.Println("No new chunks since the last time we ran, sleeping for three minutes...")
				time.Sleep(3000 * time.Millisecond)

			} else {
				fmt.Println("Processing")

				s.WasRunning = true
				s.Counter++
				s.ShowStateChange("sleep", "wake")

				var monitors []Monitor
				root := config.GetPathToCache(chain) + "monitors/"
				err := filepath.Walk(root, func(path string, info os.FileInfo, err error) error {
					if strings.Contains(path, ".acct.bin") {
						parts := strings.Split(strings.Replace(path, ".acct.bin", "", -1), "/")

						var mon Monitor
						mon.Path = path
						mon.Address = parts[len(parts)-1]
						mon.Size = uint64(info.Size())
						mon.Count = mon.Size / 8
						monitors = append(monitors, mon)
					}
					return nil
				})
				if err != nil {
					panic(err)
				}

				var nChanged uint = 0
				var nProcessed uint = 0
				for _, mon := range monitors {
					if !MonitorScraper.Running {
						break
					}
					countBefore := mon.Count

					nProcessed++
					options := " --freshen"
					options += " " + mon.Address
					opts.Globals.PassItOn("acctExport", options)
					options = " --appearances -U --no_header " + mon.Address
					opts.Globals.PassItOn("acctExport", options)
					in, err := os.Stat(mon.Path)
					if err != nil {
						log.Fatal(err)
					}
					// fmt.Println(in)
					mon.Size = uint64(in.Size())
					mon.Count = mon.Size / 8
					if countBefore < mon.Count {
						fmt.Println("\tChanged", mon.Address, mon.Path, mon.Size, mon.Count, (float64(mon.Size) / 8.0))
						nChanged++
					}
				}

				fmt.Println("nProcessed: ", nProcessed, " nChanged: ", nChanged)
				s.ShowStateChange("wake", "sleep")
				time.Sleep(time.Duration(MonitorScraper.SleepSecs) * time.Second)
			}
			first = false
			prevChunk = thisChunk
		}
	}
}

/*
        for (auto line : lines) {
            CStringArray parts;
            explode(parts, line, ',');
            address_t addr = toLower(parts[1]);

            // Ignore invalid addresses
            if ((!isAddress(addr) || isZeroAddr(addr)))
                continue;

            // Figure out how many records there are...
            string_q monitorFn = cacheFolder_monitors + addr + ".acct.bin";
            uint64_t nRecordsBefore = fileSize(monitorFn) / 8;

            // If there are too many, report the same and skip...
            if (nRecordsBefore > 100000) {
                ostringstream os;
                os << "Skipping too-large address: " << addr << " with " << nRecordsBefore << " appearances.";
                LOG_ERR(bRed, os.str(), cOff);
                appendToAsciiFile("./skipped-too-large.txt", os.str());
                continue;
            }

            nProcessed++;

            // Freshen the monitor...
            if (system(substitute(STR_CMD_LIST, "[{ADDR}]", addr).c_str()) != 0) {
                quit = true;
                break;
            }

            // Figure out how many records there are after freshen...
            uint64_t sizeAfter = fileSize(monitorFn) / 8;

            // If there is no transactions file (we can delete that file to force a re-calc) or
            // there are new transactions, re-process. Otherwise, skip...
            if (fileExists("./txs/" + addr + ".csv") && nRecordsBefore == sizeAfter) {
                // There are no new records, we don't have to freshen the rest of the data...
                LOG_INFO(bBlack, "Skip ", substitute(monitorFn, cacheFolder, "./"), bGreen, " (", nRecordsBefore,
                         " == ", sizeAfter, ")", cOff);

            } else {
                // There are new records, re-write everything...
                LOG_INFO(bYellow, "Call ", substitute(monitorFn, cacheFolder, "./"), bGreen, " (", nRecordsBefore,
                         " != ", sizeAfter, ")", cOff);

                nChanged++;

                ostringstream oss;
                oss << STR_CMD_TXS << endl;
                oss << STR_CMD_LOGS << endl;
                oss << STR_CMD_NEIGHBORS << endl;
                oss << STR_CMD_STATEMENTS << endl;
                int ret = system(substitute(oss.str(), "[{ADDR}]", addr).c_str());
                if (WIFSIGNALED(ret) && (WTERMSIG(ret) == SIGINT || WTERMSIG(ret) == SIGQUIT)) {
                    cerr << "system call interrupted" << endl;
                    break;
                } else {
                    if (ret != 0 && ret != 256) {
                        cerr << "system call returned " << ret << ". Quitting..." << endl;
                        quit = true;
                    }
                }
            }
        }  // for (auto line : lines)

        if (!quit && nChanged) {
            if (system("./combine_update.sh")) {
            }
            if (system("./update_zips.sh")) {
            }
        }
        prevChunk = thisChunk;
}

//-----------------------------------------------------------------------------------
const char* STR_CMD_LIST = "chifra export --appearances --fmt csv [{ADDR}] | cut -f2,3 -d',' >apps/[{ADDR}].csv ; ";
const char* STR_CMD_TXS = "chifra export --articulate --cache --cache_traces --fmt csv [{ADDR}] >txs/[{ADDR}].csv ; ";
const char* STR_CMD_LOGS = "./export_logs.1.sh [{ADDR}] ; ";
const char* STR_CMD_NEIGHBORS = "chifra export --neighbors --deep --fmt csv [{ADDR}] >neighbors/[{ADDR}].csv ; ";
const char* STR_CMD_STATEMENTS = "./export_statements.1.sh [{ADDR}] ; ";
*/
