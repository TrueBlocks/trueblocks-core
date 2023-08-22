package monitorsPkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/json"
	"fmt"
	"os"
	"path/filepath"
	"strings"
	"sync"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
)

var MonitorScraper Scraper

// RunMonitorScraper runs continually, never stopping and freshens any existing monitors
func (opts *MonitorsOptions) RunMonitorScraper(wg *sync.WaitGroup) {
	defer wg.Done()

	chain := opts.Globals.Chain
	establishExportPaths(chain)

	tmpPath := config.GetPathToCache(chain) + "tmp/"

	var s = &MonitorScraper
	s.ChangeState(true, tmpPath)

	for {
		if !s.Running {
			s.Pause()

		} else {
			monitorChan := make(chan monitor.Monitor)
			var monitors []monitor.Monitor
			go monitor.ListMonitors(chain, monitorChan)
			count := 0
			for result := range monitorChan {
				switch result.Address {
				case monitor.SentinalAddr:
					close(monitorChan)
				default:
					if result.Count() > 500000 {
						logger.Warn("Ignoring too-large address", result.Address)
						continue
					}
					monitors = append(monitors, result)
					count++
					if result.Count() > 0 {
						logger.Info("     ", count, ": ", result, "                                        ")
					}
				}
			}

			canceled, _ := opts.Refresh(monitors)
			if canceled || os.Getenv("RUN_ONCE") == "true" {
				return
			}

			sleep := opts.Sleep
			if sleep > 0 {
				ms := time.Duration(sleep*1000) * time.Millisecond
				if !opts.Globals.TestMode {
					logger.Info(fmt.Sprintf("Sleeping for %g seconds", sleep))
				}
				time.Sleep(ms)
			}
		}
	}
}

type SemiParse struct {
	CmdLine string `json:"cmdLine"`
	Fmt     string `json:"fmt"`
	Folder  string `json:"folder"`
}

func (sp SemiParse) String() string {
	ret, _ := json.MarshalIndent(sp, "", "  ")
	return string(ret)
}

// const addrsPerBatch = 8

func (opts *MonitorsOptions) Refresh(monitors []monitor.Monitor) (bool, error) {
	// chain := opts.Globals.Chain
	// theCmds, _ := getCommandsFromFile(opts.Globals)

	// batches := batchMonitors(monitors, addrsPerBatch)
	// for i := 0; i < len(batches); i++ {
	// 	addrs, countsBefore := preProcessBatch(batches[i], i, len(monitors))

	// 	canceled, err := opts.FreshenMonitorsForWatch(addrs)
	// 	if canceled || err != nil {
	// 		return canceled, err
	// 	}

	// 	for j := 0; j < len(batches[i]); j++ {
	// 		mon := batches[i][j]
	// 		countAfter := mon.Count()

	// 		if countAfter > 1000000 {
	// 			fmt.Println(colors.Red, "Too many transactions for address", mon.Address, colors.Off)
	// 			continue
	// 		}

	// 		if countAfter == 0 {
	// 			continue
	// 		}

	// 		for _, sp := range theCmds {
	// 			outputFn := sp.Folder + "/" + mon.Address.Hex() + "." + sp.Fmt
	// 			exists := file.FileExists(outputFn)
	// 			countBefore := countsBefore[j]

	// 			if !exists || countAfter > countBefore {
	// 				cmd := sp.CmdLine + " --output " + outputFn
	// 				add := ""
	// 				if exists {
	// 					add += fmt.Sprintf(" --first_record %d", uint64(countBefore+1))
	// 					add += fmt.Sprintf(" --max_records %d", uint64(countAfter-countBefore+1)) // extra space won't hurt
	// 					add += " --append --no_header"
	// 				}
	// 				cmd += add + " " + mon.Address.Hex()
	// 				cmd = strings.Replace(cmd, "  ", " ", -1)
	// 				o := opts
	// 				o.Globals.File = ""
	// 				_ = o.Globals.PassItOn("acctExport", chain, cmd, []string{})
	// 			}
	// 		}
	// 	}
	// }
	return false, nil
}

// func batchMonitors(slice []monitor.Monitor, batchSize int) [][]monitor.Monitor {
// 	var batches [][]monitor.Monitor
// 	for i := 0; i < len(slice); i += batchSize {
// 		end := i + batchSize
// 		if end > len(slice) {
// 			end = len(slice)
// 		}
// 		batches = append(batches, slice[i:end])
// 	}
// 	return batches
// }

func GetExportFormat(cmd, def string) string {
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

// func getCommandsFromFile(globals globals.GlobalOptions) ([]SemiParse, error) {
// 	ret := []SemiParse{}
// 	cmdLines := []string{}

// 	commandFile := globals.File
// 	if commandFile == "" && file.FileExists("./commands.fil") {
// 		commandFile = "./commands.fil"
// 	}
// 	if !file.FileExists(commandFile) {
// 		logger.Warn("No --file option supplied. Using default.")
// 		cmdLines = append(cmdLines, "export --appearances")
// 	} else {
// 		cmdLines = file.AsciiFileToLines(commandFile)
// 	}

// 	for _, cmd := range cmdLines {
// 		if strings.HasPrefix(cmd, "export ") {
// 			cmd = strings.Replace(cmd, "export ", "", -1)
// 		}
// 		cmd = strings.Trim(cmd, " \t")
// 		if len(cmd) > 0 && !strings.HasPrefix(cmd, "#") {
// 			sp := SemiParse{}
// 			sp.Folder = "exports/" + globals.Chain + "/" + GetOutputFolder(cmd, "unknown")

// 			sp.Fmt = GetExportFormat(cmd, globals.Format)
// 			sp.CmdLine = cmd
// 			sp.CmdLine = strings.Replace(sp.CmdLine, " csv", " "+sp.Fmt, -1)
// 			sp.CmdLine = strings.Replace(sp.CmdLine, " json", " "+sp.Fmt, -1)
// 			sp.CmdLine = strings.Replace(sp.CmdLine, " txt", " "+sp.Fmt, -1)
// 			if !strings.Contains(sp.CmdLine, "--fmt") {
// 				sp.CmdLine += " --fmt " + sp.Fmt
// 			}
// 			ret = append(ret, sp)
// 		}
// 	}

// 	logger.Info("Found", len(ret), "commands to process in ./"+globals.File)
// 	for i, cmd := range ret {
// 		msg := fmt.Sprintf("\t%d. %s", i, cmd.CmdLine)
// 		logger.Info(msg)
// 	}

// 	return ret, nil
// }

// const spaces = "                                                                                 "

// func preProcessBatch(batch []monitor.Monitor, i, nMons int) ([]string, []int64) {
// 	var addrs []string
// 	for j := 0; j < len(batch); j++ {
// 		addrs = append(addrs, batch[j].Address.Hex())
// 	}

// 	fmt.Println(strings.Repeat(" ", 120), "\r")
// 	s := fmt.Sprintf("%s\r%d-%d", colors.BrightBlue+spaces, i*addrsPerBatch, nMons)
// 	fmt.Println(s, colors.Green, "chifra export --freshen", strings.Replace(strings.Join(addrs, " "), "0x", " \\\n\t0x", -1), colors.Off)

// 	countsBefore := []int64{}
// 	for j := 0; j < len(batch); j++ {
// 		countsBefore = append(countsBefore, (batch)[j].Count())
// 	}

// 	return addrs, countsBefore
// }

// TODO: We could add statistics counting -- nChanged, nProcessed, txCount, etc
// TODO: Need to protect against invalid addresses including zero address
// TODO: Need to protect against very large files (optionally) --> keep a list of them?
// TODO: Does control+c work right? Does data get corrupted?
// TODO: We need post processing? Summarization? Coallessing? Zip files?

// establishExportPaths sets up the index path and subfolders. It only returns if it succeeds.
func establishExportPaths(chain string) {
	folders := []string{
		"apps",
		"logs",
		"transactions",
		"neighbors",
		"statements",
		"raw",
		"combined",
		"zips",
		"neighbors/networks",
		"neighbors/adjacencies",
		"neighbors/images",
		"neighbors/images/pngs",
		"statements/tx_counts",
		"statements/balances",
		"statements/balances/plots",
	}

	cwd, _ := os.Getwd()
	exportPath := cwd + "/exports/"
	if err := file.EstablishFolders(exportPath, nil); err != nil {
		logger.Fatal(err)
	}

	exportPath = cwd + "/exports/" + chain + "/"
	_, err := os.Stat(filepath.Join(exportPath, folders[len(folders)-1]))
	if err == nil {
		// If the last path already exists, assume we've been here before
		return
	}

	if err := file.EstablishFolders(exportPath, folders); err != nil {
		logger.Fatal(err)
	}
}

func GetOutputFolder(cmdLine, def string) string {
	cmdLine += " "
	cmdLine = strings.Replace(cmdLine, "-p ", "--appearances ", -1)
	cmdLine = strings.Replace(cmdLine, "-r ", "--receipts ", -1)
	cmdLine = strings.Replace(cmdLine, "-l ", "--logs ", -1)
	cmdLine = strings.Replace(cmdLine, "-t ", "--traces ", -1)
	cmdLine = strings.Replace(cmdLine, "-A ", "--statements ", -1)
	cmdLine = strings.Replace(cmdLine, "-n ", "--neighbors ", -1)
	cmdLine = strings.Replace(cmdLine, "-C ", "--accounting ", -1)

	if strings.Contains(cmdLine, "--appearances") {
		return "apps"
	} else if strings.Contains(cmdLine, "--receipts") {
		return "receipts"
	} else if strings.Contains(cmdLine, "--logs") {
		return "logs"
	} else if strings.Contains(cmdLine, "--traces") {
		return "traces"
	} else if strings.Contains(cmdLine, "--statements") {
		return "statements"
	} else if strings.Contains(cmdLine, "--neighbors") {
		return "neighbors"
	} else if strings.Contains(cmdLine, "--accounting") {
		return "accounting"
	}
	return "transactions"
}
