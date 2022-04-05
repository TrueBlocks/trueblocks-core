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

	exportPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/export"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
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
					if result.Count > 100000 {
						fmt.Println("Ignoring too-large address", result.Address)
						continue
					}
					monitors = append(monitors, result)
					count++
					if result.Count > 0 {
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

func chunkMonitors(slice []monitor.Monitor, chunkSize int) [][]monitor.Monitor {
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

func (opts *ScrapeOptions) Refresh(chain string, monitors []monitor.Monitor) error {
	addrsPerCall := 8

	batches := chunkMonitors(monitors, addrsPerCall)
	for i := 0; i < len(batches); i++ {
		var addrs []string
		for j := 0; j < len(batches[i]); j++ {
			addrs = append(addrs, batches[i][j].GetAddrStr())
		}
		addrStr := strings.Join(addrs, " ")

		spaces := "                                                                                 \r"
		s := fmt.Sprintf("%s\r%d-%d", colors.Blue+colors.Bright+spaces, i*addrsPerCall, len(monitors))
		fmt.Println(s, colors.Green, "chifra export --freshen", strings.Replace(addrStr, "0x", " \\\n\t0x", -1), colors.Off)

		opts.FreshenMonitors(addrs)
		for j := 0; j < len(batches[i]); j++ {

			mon := batches[i][j]
			countBefore := mon.Count
			countAfter, _ := mon.Reload(true /* create */)

			if countAfter > 0 {
				if countAfter > 100000 {
					fmt.Println("Too many transactions for address", mon.Address)
					continue
				}

				appsPath := "exports/apps/" + mon.GetAddrStr() + ".csv"
				exists := file.FileExists(appsPath)
				if !exists || countAfter > countBefore {
					start := countBefore + 1
					if !exists {
						start = 0
					}
					expApps, _ := getExportOpts(&mon, chain, appsPath, start, countAfter)
					expApps.Appearances = true
					expApps.Globals.PassItOn("acctExport", expApps.ToCmdLine())
				}

				txsPath := "exports/txs/" + mon.GetAddrStr() + ".csv"
				exists = file.FileExists(txsPath)
				if !exists || countAfter > countBefore {
					start := countBefore + 1
					if !exists {
						start = 0
					}
					expTxs, _ := getExportOpts(&mon, chain, txsPath, start, countAfter)
					expTxs.Cache = true
					expTxs.CacheTraces = true
					expTxs.Globals.PassItOn("acctExport", expTxs.ToCmdLine())
				}

				logsPath := "exports/logs/" + mon.GetAddrStr() + ".csv"
				exists = file.FileExists(logsPath)
				if !exists || countAfter > countBefore {
					start := countBefore + 1
					if !exists {
						start = 0
					}
					expLogs, _ := getExportOpts(&mon, chain, logsPath, start, countAfter)
					expLogs.Logs = true
					expLogs.Relevant = true
					expLogs.Articulate = true
					// TODO: BOGUS
					// expLogs.Emitter = append(expLogs.Emitter, "0xdf869fad6db91f437b59f1edefab319493d4c4ce")
					// expLogs.Emitter = append(expLogs.Emitter, "0x7d655c57f71464b6f83811c55d84009cd9f5221c")
					// expLogs.Emitter = append(expLogs.Emitter, "0xf2354570be2fb420832fb7ff6ff0ae0df80cf2c6")
					// expLogs.Emitter = append(expLogs.Emitter, "0x3342e3737732d879743f2682a3953a730ae4f47c")
					// expLogs.Emitter = append(expLogs.Emitter, "0x3ebaffe01513164e638480404c651e885cca0aa4")
					expLogs.Globals.PassItOn("acctExport", expLogs.ToCmdLine())
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

// getExportOpts creates an ExportOptions item to export from the first to last appearance found in the file. firstPos and lastPos are one-based
func getExportOpts(mon *monitor.Monitor, chain, path string, firstPos, lastPos uint32) (exportPkg.ExportOptions, error) {
	expOpts := exportPkg.ExportOptions{MaxRecords: 250, MaxTraces: 250}
	expOpts.Addrs = append(expOpts.Addrs, mon.GetAddrStr())
	expOpts.Globals.Chain = chain
	expOpts.Globals.Format = "csv"
	expOpts.Globals.OutputFn = path
	expOpts.Globals.Append = file.FileExists(expOpts.Globals.OutputFn)
	expOpts.Globals.NoHeader = file.FileExists(expOpts.Globals.OutputFn)

	var app index.AppearanceRecord
	err := mon.ReadAppearanceAt(firstPos, &app)
	if err != nil {
		return exportPkg.ExportOptions{}, err
	}
	expOpts.FirstBlock = uint64(app.BlockNumber)

	err = mon.ReadAppearanceAt(lastPos, &app)
	if err != nil {
		return exportPkg.ExportOptions{}, err
	}
	expOpts.LastBlock = uint64(app.BlockNumber)

	return expOpts, nil
}
