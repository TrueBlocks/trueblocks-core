package monitorsPkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/json"
	"fmt"
	"log"
	"os"
	"path"
	"strings"
	"sync"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/scraper"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

var MonitorScraper scraper.Scraper

// RunMonitorScraper runs continually, never stopping and freshens any existing monitors
func (opts *MonitorsOptions) RunMonitorScraper(wg *sync.WaitGroup) {
	defer wg.Done()

	chain := opts.Globals.Chain
	establishExportPaths(chain)

	var s *scraper.Scraper = &MonitorScraper
	s.ChangeState(true)

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
						logger.Log(logger.Info, "     ", count, ": ", result, "                                        ")
					}
				}
			}

			opts.Refresh(monitors)

			if os.Getenv("RUN_ONCE") == "true" {
				return
			}

			fmt.Println("Sleeping for", opts.Sleep, "seconds.")
			time.Sleep(time.Duration(opts.Sleep) * time.Second)
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

const addrsPerBatch = 8

func (opts *MonitorsOptions) Refresh(monitors []monitor.Monitor) error {
	theCmds, _ := getCommandsFromFile(opts.Globals)

	batches := batchMonitors(monitors, addrsPerBatch)
	for i := 0; i < len(batches); i++ {
		addrs, countsBefore := preProcessBatch(batches[i], i, len(monitors))

		err := opts.FreshenMonitorsScrape(addrs)
		if err != nil {
			return err
		}

		for j := 0; j < len(batches[i]); j++ {
			mon := batches[i][j]
			countAfter := mon.Count()

			if countAfter > 100000 {
				fmt.Println(colors.Red, "Too many transactions for address", mon.Address, colors.Off)
				continue
			}

			if countAfter == 0 {
				continue
			}

			for _, sp := range theCmds {
				outputFn := sp.Folder + "/" + mon.GetAddrStr() + "." + sp.Fmt
				exists := file.FileExists(outputFn)
				countBefore := countsBefore[j]

				if !exists || countAfter > countBefore {
					cmd := sp.CmdLine + " --output " + outputFn
					add := ""
					if exists {
						add += fmt.Sprintf(" --first_record %d", uint64(countBefore+1))
						add += fmt.Sprintf(" --max_records %d", uint64(countAfter-countBefore+1)) // extra space won't hurt
						add += fmt.Sprintf(" --append")
						add += fmt.Sprintf(" --no_header")
					}
					cmd += add + " " + mon.GetAddrStr()
					cmd = strings.Replace(cmd, "  ", " ", -1)
					o := opts
					o.Globals.File = ""
					o.Globals.PassItOn("acctExport", opts.Globals.Chain, cmd, opts.Globals.ToCmdLine())
					// fmt.Println("Processing:", colors.BrightYellow, outputFn, colors.BrightWhite, exists, countBefore, countAfter, colors.Off)
					// } else {
					// 	fmt.Println("Skipping:", colors.BrightYellow, outputFn, colors.BrightWhite, exists, countBefore, countAfter, colors.Off)
				}
				// time.Sleep(500 * time.Millisecond)
			}
		}
	}
	return nil
}

func batchMonitors(slice []monitor.Monitor, batchSize int) [][]monitor.Monitor {
	var batches [][]monitor.Monitor
	for i := 0; i < len(slice); i += batchSize {
		end := i + batchSize
		if end > len(slice) {
			end = len(slice)
		}
		batches = append(batches, slice[i:end])
	}
	return batches
}

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

func getCommandsFromFile(globals globals.GlobalOptions) ([]SemiParse, error) {
	if !file.FileExists(globals.File) {
		return []SemiParse{}, fmt.Errorf("file %s not found", globals.File)
	}

	ret := []SemiParse{}

	cmdLines := utils.AsciiFileToLines(globals.File)
	for _, cmd := range cmdLines {
		if strings.HasPrefix(cmd, "export ") {
			cmd = strings.Replace(cmd, "export ", "", -1)
		}
		cmd = strings.Trim(cmd, " \t")
		if len(cmd) > 0 && !strings.HasPrefix(cmd, "#") {
			sp := SemiParse{}
			sp.Folder = "exports/" + globals.Chain + "/" + GetOutputFolder(cmd, "unknown")

			sp.Fmt = GetExportFormat(cmd, globals.Format)
			sp.CmdLine = cmd
			sp.CmdLine = strings.Replace(sp.CmdLine, " csv", " "+sp.Fmt, -1)
			sp.CmdLine = strings.Replace(sp.CmdLine, " json", " "+sp.Fmt, -1)
			sp.CmdLine = strings.Replace(sp.CmdLine, " txt", " "+sp.Fmt, -1)
			if !strings.Contains(sp.CmdLine, "--fmt") {
				sp.CmdLine += " --fmt " + sp.Fmt
			}
			ret = append(ret, sp)
		}
	}

	logger.Log(logger.Info, "Found", len(ret), "commands to process in ./"+globals.File)
	for i, cmd := range ret {
		msg := fmt.Sprintf("\t%d. %s", i, cmd.CmdLine)
		logger.Log(logger.Info, msg)
	}

	return ret, nil
}

const spaces = "                                                                                 "

func preProcessBatch(batch []monitor.Monitor, i, nMons int) ([]string, []uint32) {
	var addrs []string
	for j := 0; j < len(batch); j++ {
		addrs = append(addrs, batch[j].GetAddrStr())
	}

	s := fmt.Sprintf("%s\r%d-%d", colors.BrightBlue+spaces, i*addrsPerBatch, nMons)
	fmt.Println(s, colors.Green, "chifra export --freshen", strings.Replace(strings.Join(addrs, " "), "0x", " \\\n\t0x", -1), colors.Off)

	countsBefore := []uint32{}
	for j := 0; j < len(batch); j++ {
		countsBefore = append(countsBefore, (batch)[j].Count())
	}

	return addrs, countsBefore
}

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
		"txs",
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
		log.Fatal(err)
	}

	exportPath = cwd + "/exports/" + chain + "/"
	_, err := os.Stat(path.Join(exportPath, folders[len(folders)-1]))
	if err == nil {
		// If the last path already exists, assume we've been here before
		return
	}

	if err := file.EstablishFolders(exportPath, folders); err != nil {
		log.Fatal(err)
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
	return "txs"
}
