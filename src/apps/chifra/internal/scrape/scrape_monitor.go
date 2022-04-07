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
					expOpts := exportPkg.ExportOptions{
						Addrs: append([]string{}, mon.GetAddrStr()),
						// Topics:      append([]string{}, ""),
						// Fourbytes:   append([]string{}, ""),
						Appearances: true,
						// Receipts:    false,
						// Logs:        false,
						// Traces:      false,
						// Statements:  false,
						// Neighbors:   false,
						// Accounting:  false,
						// Articulate:  false,
						// Cache:       false,
						// CacheTraces: false,
						// Count:       false,
						FirstRecord: uint64(countBefore + 1),
						MaxRecords:  uint64(countAfter - countBefore + 1), // extra space won't hurt
						// Relevant:    false,
						// Emitter:     append([]string{}, ""),
						// Topic:       append([]string{}, ""),
						// Asset:       append([]string{}, ""),
						// Factory:     false,
						// Staging:     false,
						// Unripe:      false,
						// Load:        "",
						// Reversed:    false,
						// SkipDdos:    false,
						MaxTraces:  250,
						FirstBlock: 0,
						LastBlock:  utils.NOPOS,
						Globals:    opts.Globals,
						// BadFlag:    nil,
					}
					if !exists {
						expOpts.FirstRecord = 0
					}
					expOpts.Globals.OutputFn = appsPath
					expOpts.Globals.Append = file.FileExists(expOpts.Globals.OutputFn)
					expOpts.Globals.NoHeader = file.FileExists(expOpts.Globals.OutputFn)
					expOpts.Globals.Format = "csv"
					expOpts.Globals.File = ""
					expOpts.Globals.PassItOn("acctExport", expOpts.ToCmdLine())
				}

				txsPath := "exports/txs/" + mon.GetAddrStr() + ".csv"
				exists = file.FileExists(txsPath)
				if !exists || countAfter > countBefore {
					expOpts := exportPkg.ExportOptions{
						Addrs: append([]string{}, mon.GetAddrStr()),
						// Topics:      append([]string{}, ""),
						// Fourbytes:   append([]string{}, ""),
						// Appearances: false,
						// Receipts:    false,
						// Logs:        false,
						// Traces:      false,
						// Statements:  false,
						// Neighbors:   false,
						// Accounting:  false,
						Articulate:  true,
						Cache:       true,
						CacheTraces: true,
						// Count:       false,
						FirstRecord: uint64(countBefore + 1),
						MaxRecords:  uint64(countAfter - countBefore + 1), // extra space won't hurt
						// Relevant:    false,
						// Emitter:     append([]string{}, ""),
						// Topic:       append([]string{}, ""),
						// Asset:       append([]string{}, ""),
						// Factory:     false,
						// Staging:     false,
						// Unripe:      false,
						// Load:        "",
						// Reversed:    false,
						// SkipDdos:    false,
						MaxTraces:  250,
						FirstBlock: 0,
						LastBlock:  utils.NOPOS,
						Globals:    opts.Globals,
						// BadFlag:    nil,
					}
					if !exists {
						expOpts.FirstRecord = 0
					}
					expOpts.Globals.OutputFn = txsPath
					expOpts.Globals.Append = file.FileExists(expOpts.Globals.OutputFn)
					expOpts.Globals.NoHeader = file.FileExists(expOpts.Globals.OutputFn)
					expOpts.Globals.Format = "csv"
					expOpts.Globals.File = ""
					expOpts.Globals.PassItOn("acctExport", expOpts.ToCmdLine())
				}

				logsPath := "exports/logs/" + mon.GetAddrStr() + ".csv"
				exists = file.FileExists(logsPath)
				if !exists || countAfter > countBefore {
					expOpts := exportPkg.ExportOptions{
						Addrs: append([]string{}, mon.GetAddrStr()),
						// Topics:      append([]string{}, ""),
						// Fourbytes:   append([]string{}, ""),
						// Appearances: false,
						// Receipts:    false,
						Logs: true,
						// Traces:      false,
						// Statements:  false,
						// Neighbors:   false,
						// Accounting:  false,
						// Articulate:  true,
						// Cache:       false,
						// CacheTraces: false,
						// Count:       false,
						FirstRecord: uint64(countBefore + 1),
						MaxRecords:  uint64(countAfter - countBefore + 1), // extra space won't hurt
						// Relevant:    true,
						// Emitter:     append([]string{}, ""),
						// Topic:       append([]string{}, ""),
						// Asset:       append([]string{}, ""),
						// Factory:     false,
						// Staging:     false,
						// Unripe:      false,
						// Load:        "",
						// Reversed:    false,
						// SkipDdos:    false,
						MaxTraces:  250,
						FirstBlock: 0,
						LastBlock:  utils.NOPOS,
						Globals:    opts.Globals,
						// BadFlag:    nil,
					}
					if !exists {
						expOpts.FirstRecord = 0
					}
					expOpts.Globals.OutputFn = logsPath
					expOpts.Globals.Append = file.FileExists(expOpts.Globals.OutputFn)
					expOpts.Globals.NoHeader = file.FileExists(expOpts.Globals.OutputFn)
					expOpts.Globals.Format = "csv"
					expOpts.Globals.File = ""
					expOpts.Emitter = append(expOpts.Emitter, "0xdf869fad6db91f437b59f1edefab319493d4c4ce")
					// expOpts.Emitter = append(expOpts.Emitter, "0x7d655c57f71464b6f83811c55d84009cd9f5221c")
					// expOpts.Emitter = append(expOpts.Emitter, "0xf2354570be2fb420832fb7ff6ff0ae0df80cf2c6")
					// expOpts.Emitter = append(expOpts.Emitter, "0x3342e3737732d879743f2682a3953a730ae4f47c")
					// expOpts.Emitter = append(expOpts.Emitter, "0x3ebaffe01513164e638480404c651e885cca0aa4")
					expOpts.Globals.PassItOn("acctExport", expOpts.ToCmdLine())
				}
				// 	// TODO: BOGUS
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
