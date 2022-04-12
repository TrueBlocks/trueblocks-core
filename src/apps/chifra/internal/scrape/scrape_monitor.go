// TODO: BOGUS - MUST REMOVE OLD .acct.bin files and auto upgrade to .mon.bin file

package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"log"
	"os"
	"path"
	"strings"
	"sync"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func hasMonitorsFlag(mode string) bool {
	return mode == "monitors" || mode == "both"
}

var MonitorScraper Scraper

// RunMonitorScraper runs continually, never stopping and freshens any existing monitors
func (opts *ScrapeOptions) RunMonitorScraper(wg *sync.WaitGroup, initialState bool) {
	defer wg.Done()

	chain := opts.Globals.Chain
	establishExportPaths()

	var s *Scraper = &MonitorScraper
	s.ChangeState(initialState)

	for {
		if !s.Running {
			s.Pause()

		} else {
			monitorChan := make(chan monitor.Monitor)

			var monitors []monitor.Monitor
			go monitor.ListMonitors(chain, "monitors", monitorChan)

			count := 0
			for result := range monitorChan {
				switch result.Address {
				case monitor.SentinalAddr:
					close(monitorChan)
				default:
					if result.Count() > 100000 {
						fmt.Println("Ignoring too-large address", result.Address)
						continue
					}
					monitors = append(monitors, result)
					count++
					if result.Count() > 0 {
						fmt.Println("     ", count, ": ", result, "                                        ")
					}
				}
			}
			opts.Refresh(chain, monitors)
			fmt.Println("Sleeping for", opts.Sleep, "seconds.")
			time.Sleep(time.Duration(opts.Sleep) * time.Second)
		}
	}
}

func batchMonitors(slice []monitor.Monitor, chunkSize int) [][]monitor.Monitor {
	var chunks [][]monitor.Monitor
	for i := 0; i < len(slice); i += chunkSize {
		end := i + chunkSize
		if end > len(slice) {
			end = len(slice)
		}
		chunks = append(chunks, slice[i:end])
	}
	return chunks
}

func getOutputFolder(cmd, def string) string {
	cmd += " "
	cmd = strings.Replace(cmd, "-p ", "--appearances ", -1)
	cmd = strings.Replace(cmd, "-r ", "--receipts ", -1)
	cmd = strings.Replace(cmd, "-l ", "--logs ", -1)
	cmd = strings.Replace(cmd, "-t ", "--traces ", -1)
	cmd = strings.Replace(cmd, "-A ", "--statements ", -1)
	cmd = strings.Replace(cmd, "-n ", "--neighbors ", -1)
	cmd = strings.Replace(cmd, "-C ", "--accounting ", -1)

	if strings.Contains(cmd, "--appearances") {
		return "apps"
	} else if strings.Contains(cmd, "--receipts") {
		return "receipts"
	} else if strings.Contains(cmd, "--logs") {
		return "logs"
	} else if strings.Contains(cmd, "--traces") {
		return "traces"
	} else if strings.Contains(cmd, "--statements") {
		return "statements"
	} else if strings.Contains(cmd, "--neighbors") {
		return "neighbors"
	} else if strings.Contains(cmd, "--accounting") {
		return "accounting"
	}
	return "txs"
}

func getFormat(cmd, def string) string {
	if strings.Contains(cmd, "json") {
		return "json"
	} else if strings.Contains(cmd, "txt") {
		return "txt"
	} else if strings.Contains(cmd, "csv") {
		return "csv"
	}
	if len(def) > 0 {
		return def
	}
	return "csv"
}

type SemiParse struct {
	cmd    string
	fmt    string
	folder string
}

func (opts *ScrapeOptions) Refresh(chain string, monitors []monitor.Monitor) error {
	cmdFile := opts.Globals.File
	contents := utils.AsciiFileToString(cmdFile)
	cmds := strings.Split(contents, "\n")
	theCmds := []SemiParse{}
	for _, cmd := range cmds {
		cmd = strings.Trim(cmd, " \t")
		if len(cmd) > 0 && !strings.HasPrefix(cmd, "#") {
			sp := SemiParse{}
			sp.fmt = getFormat(cmd, opts.Globals.Format)
			sp.folder = "exports/" + getOutputFolder(cmd, "unknown")
			sp.cmd = strings.Replace(cmd, " csv ", " "+sp.fmt+" ", -1)
			sp.cmd = strings.Replace(cmd, " json ", " "+sp.fmt+" ", -1)
			sp.cmd = strings.Replace(cmd, " txt ", " "+sp.fmt+" ", -1)
			if !strings.Contains(sp.cmd, "--fmt") {
				sp.cmd += " --fmt " + sp.fmt
			}
			theCmds = append(theCmds, sp)
		}
	}

	fmt.Println("Found", len(theCmds), "commands to process in ./"+cmdFile)
	for i, cmd := range theCmds {
		fmt.Printf("\t%d. %s\n", i, cmd.cmd)
	}

	addrsPerCall := 8

	batches := batchMonitors(monitors, addrsPerCall)
	for i := 0; i < len(batches); i++ {
		var addrs []string
		for j := 0; j < len(batches[i]); j++ {
			addrs = append(addrs, batches[i][j].GetAddrStr())
		}
		addrStr := strings.Join(addrs, " ")

		spaces := "                                                                                 \r"
		s := fmt.Sprintf("%s\r%d-%d", colors.Blue+colors.Bright+spaces, i*addrsPerCall, len(monitors))
		fmt.Println(s, colors.Green, "chifra export --freshen", strings.Replace(addrStr, "0x", " \\\n\t0x", -1), colors.Off)

		countsBefore := []uint32{}
		for j := 0; j < len(batches[i]); j++ {
			countsBefore = append(countsBefore, batches[i][j].Count())
		}
		opts.FreshenMonitors(addrs)

		for j := 0; j < len(batches[i]); j++ {

			mon := batches[i][j]
			countBefore := countsBefore[j]
			countAfter := mon.Count()

			if countAfter > 0 {
				if countAfter > 100000 {
					fmt.Println(colors.Red, "Too many transactions for address", mon.Address, colors.Off)
					continue
				}

				for _, sp := range theCmds {
					cmd := sp.cmd + " --output " + sp.folder + "/" + mon.GetAddrStr() + "." + sp.fmt
					appsPath := sp.folder + "/" + mon.GetAddrStr() + "." + sp.fmt
					exists := file.FileExists(appsPath)
					if !exists || countAfter > countBefore {
						add := ""
						if exists {
							add += fmt.Sprintf(" --first_record %d", uint64(countBefore+1))
						} else {
							add += fmt.Sprintf(" --first_record %d", 0)
						}
						add += fmt.Sprintf(" --max_records %d", uint64(countAfter-countBefore+1)) // extra space won't hurt
						// add += fmt.Sprintf(" --max_traces 250")
						// add += fmt.Sprintf(" --first_block 0")
						// add += fmt.Sprintf(" --last_block %d", utils.NOPOS)
						if exists {
							add += fmt.Sprintf(" --append")
							add += fmt.Sprintf(" --no_header")
						}
						cmd += add + " " + mon.GetAddrStr()
						cmd = strings.Replace(cmd, "  ", " ", -1)
						// fmt.Println(cmd)
						// time.Sleep(3 * time.Second)
						o := opts
						o.Globals.File = ""
						o.Globals.PassItOn("acctExport", cmd)
					}
				}
			}
		}
	}
	return nil
}

// TODO: We could add statistics counting -- nChanged, nProcessed, txCount, etc
// TODO: Need to protect against invalid addresses including zero address
// TODO: Need to protect against very large files (optionally) --> keep a list of them?
// TODO: Does control+c work right? Does data get corrupted?
// TODO: We need post processing? Summarization? Coallessing? Zip files?

// const char* STR_CMD_LOGS = "./export_logs.1.sh [{ADDR}] ; ";
// const char* STR_CMD_NEIGHBORS = "chifra export --neighbors --deep --fmt csv [{ADDR}] >neighbors/[{ADDR}].csv ; ";
// const char* STR_CMD_STATEMENTS = "./export_statements.1.sh [{ADDR}] ; ";

// cat /tmp/$addr.csv | cut -f1-10 -d, >logs/$addr.csv
// cat /tmp/$addr.csv | cut -f1-5,11-2000 -d, >logs/articulated/$addr.csv
// rm -f /tmp/$addr.csv

// STATEMENTS
// chifra export --statements --fmt csv $addr >statements/$addr.csv
// cat statements/$addr.csv | cut -d, -f1,2,3,4,5,6,9,25,26,30-33 | tee statements/balances/$addr.csv
// echo "count,assetAddr,assetSymbol" | tee statements/tx_counts/$addr.csv
// cat statements/balances/$addr.csv | grep -v assetAddr | cut -d, -f1,2 | sort | uniq -c | sort -n -r | sed 's/ //g' | sed 's/"/,/g' | cut -d, -f1,2,5 | tee -a statements/tx_counts/$addr.csv

// establishExportPaths sets up the index path and subfolders. It only returns if it succeeds.
func establishExportPaths() {
	folders := []string{
		"txs", "apps", "statements", "neighbors", "logs", "combined", "zips",
	}

	cwd, _ := os.Getwd()

	exportPath := cwd + "/exports/"

	_, err := os.Stat(path.Join(exportPath, folders[len(folders)-1]))
	if err == nil {
		// If the last path already exists, assume we've been here before
		return
	}

	if err := file.EstablishFolders(exportPath, folders); err != nil {
		log.Fatal(err)
	}
}
