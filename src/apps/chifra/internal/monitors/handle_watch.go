// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"encoding/json"
	"fmt"
	"path/filepath"
	"strings"
	"sync"
	"time"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// HandleWatch starts the monitor watcher
func (opts *MonitorsOptions) HandleWatch(rCtx *output.RenderCtx) error {
	opts.Globals.Cache = true
	scraper := NewScraper(colors.Magenta, "MonitorScraper", opts.Sleep, 0)

	var wg sync.WaitGroup
	wg.Add(1)
	// Note that this never returns in normal operation
	go opts.RunMonitorScraper(&wg, &scraper)
	wg.Wait()

	return nil
}

// RunMonitorScraper runs continually, never stopping and freshens any existing monitors
func (opts *MonitorsOptions) RunMonitorScraper(wg *sync.WaitGroup, s *Scraper) {
	defer wg.Done()

	chain := opts.Globals.Chain
	tmpPath := filepath.Join(config.PathToCache(chain), "tmp") + string(os.PathSeparator)

	s.ChangeState(true, tmpPath)

	runCount := uint64(0)
	for {
		if !s.Running {
			s.Pause()

		} else {
			monitorList := opts.getMonitorList()
			if len(monitorList) == 0 {
				logger.Error(validate.Usage("No monitors found. Use 'chifra list' to initialize a monitor.").Error())
				return
			}

			if canceled, err := opts.Refresh(monitorList); err != nil {
				logger.Error(err)
				return
			} else {
				if canceled {
					return
				}
			}

			runCount++
			if opts.RunCount != 0 && runCount >= opts.RunCount {
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

type Command struct {
	Fmt    string `json:"fmt"`
	Folder string `json:"folder"`
	Cmd    string `json:"cmd"`
	Cache  bool   `json:"cache"`
}

func (c *Command) fileName(addr base.Address) string {
	return filepath.Join(c.Folder, addr.Hex()+"."+c.Fmt)
}

func (c *Command) resolve(addr base.Address, before, after int64) string {
	fn := c.fileName(addr)
	if file.FileExists(fn) {
		if strings.Contains(c.Cmd, "export") {
			c.Cmd += fmt.Sprintf(" --first_record %d", uint64(before+1))
			c.Cmd += fmt.Sprintf(" --max_records %d", uint64(after-before+1)) // extra space won't hurt
		} else {
			c.Cmd += fmt.Sprintf(" %d-%d", before+1, after)
		}
		c.Cmd += " --append --no_header"
	}
	c.Cmd = strings.Replace(c.Cmd, "  ", " ", -1)
	ret := c.Cmd + " --fmt " + c.Fmt + " --output " + c.fileName(addr) + " " + addr.Hex()
	if c.Cache {
		ret += " --cache"
	}
	return ret
}

func (c *Command) String() string {
	b, _ := json.MarshalIndent(c, "", "  ")
	return string(b)
}

func (opts *MonitorsOptions) Refresh(monitors []monitor.Monitor) (bool, error) {
	theCmds, err := opts.getCommands()
	if err != nil {
		return false, err
	}

	batches := batchSlice[monitor.Monitor](monitors, opts.BatchSize)
	for i := 0; i < len(batches); i++ {
		addrs := []base.Address{}
		countsBefore := []int64{}
		for _, mon := range batches[i] {
			addrs = append(addrs, mon.Address)
			countsBefore = append(countsBefore, mon.Count())
		}

		batchSize := int(opts.BatchSize)
		fmt.Printf("%s%d-%d of %d:%s chifra export --freshen",
			colors.BrightBlue,
			i*batchSize,
			base.Min(((i+1)*batchSize)-1, len(monitors)),
			len(monitors),
			colors.Green)
		for _, addr := range addrs {
			fmt.Printf(" %s", addr.Hex())
		}
		fmt.Println(colors.Off)

		canceled, err := opts.FreshenMonitorsForWatch(addrs)
		if canceled || err != nil {
			return canceled, err
		}

		for j := 0; j < len(batches[i]); j++ {
			mon := batches[i][j]
			countAfter := mon.Count()

			if countAfter > 1000000 {
				// TODO: Make this value configurable
				fmt.Println(colors.Red, "Too many transactions for address", mon.Address, colors.Off)
				continue
			}

			if countAfter == 0 {
				continue
			}

			logger.Info(fmt.Sprintf("Processing item %d in batch %d: %d %d\n", j, i, countsBefore[j], countAfter))

			for _, cmd := range theCmds {
				countBefore := countsBefore[j]
				if countBefore == 0 || countAfter > countBefore {
					utils.System(cmd.resolve(mon.Address, countBefore, countAfter))
					// o := opts
					// o.Globals.File = ""
					// _ = o.Globals.PassItOn("acctExport", chain, cmd, []string{})
				} else if opts.Globals.Verbose {
					fmt.Println("No new transactions for", mon.Address.Hex(), "since last run.")
				}
			}
		}
	}
	return false, nil
}

func batchSlice[T any](slice []T, batchSize uint64) [][]T {
	var batches [][]T
	for i := 0; i < len(slice); i += int(batchSize) {
		end := i + int(batchSize)
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

func (opts *MonitorsOptions) cleanLine(lineIn string) (cmd Command, err error) {
	line := strings.Replace(lineIn, "[{ADDRESS}]", "", -1)
	if strings.Contains(line, "--fmt") {
		line = strings.Replace(line, "--fmt", "", -1)
		line = strings.Replace(line, "json", "", -1)
		line = strings.Replace(line, "csv", "", -1)
		line = strings.Replace(line, "txt", "", -1)
	}
	line = utils.StripComments(line)
	if len(line) == 0 {
		return Command{}, nil
	}

	folder, err := opts.getOutputFolder(line)
	if err != nil {
		return Command{}, err
	}

	_ = file.EstablishFolder(folder)
	return Command{Cmd: line, Folder: folder, Fmt: GetExportFormat(lineIn, "csv"), Cache: opts.Globals.Cache}, nil
}

func (opts *MonitorsOptions) getCommands() (ret []Command, err error) {
	lines := file.AsciiFileToLines(opts.Commands)
	for _, line := range lines {
		// orig := line
		if cmd, err := opts.cleanLine(line); err != nil {
			return nil, err
		} else if len(cmd.Cmd) == 0 {
			continue
		} else {
			ret = append(ret, cmd)
		}
	}
	return ret, nil
}

func (opts *MonitorsOptions) getOutputFolder(orig string) (string, error) {
	okMap := map[string]bool{
		"export": true,
		"list":   true,
		"state":  true,
		"tokens": true,
	}

	cmdLine := orig
	parts := strings.Split(strings.Replace(cmdLine, "  ", " ", -1), " ")
	if len(parts) < 1 || parts[0] != "chifra" {
		s := fmt.Sprintf("Invalid command: %s. Must start with 'chifra'.", strings.Trim(orig, " \t\n\r"))
		logger.Fatal(s)
	}
	if len(parts) < 2 || !okMap[parts[1]] {
		s := fmt.Sprintf("Invalid command: %s. Must start with 'chifra export', 'chifra list', 'chifra state', or 'chifra tokens'.", orig)
		logger.Fatal(s)
	}

	cmdLine += " "
	folder := "unknown"
	if parts[1] == "export" {
		folder = "./export"
		if strings.Contains(cmdLine, "-p ") || strings.Contains(cmdLine, "--appearances ") {
			folder += "/appearances"
		} else if strings.Contains(cmdLine, "-r ") || strings.Contains(cmdLine, "--receipts ") {
			folder += "/receipts"
		} else if strings.Contains(cmdLine, "-l ") || strings.Contains(cmdLine, "--logs ") {
			folder += "/logs"
		} else if strings.Contains(cmdLine, "-t ") || strings.Contains(cmdLine, "--traces ") {
			folder += "/traces"
		} else if strings.Contains(cmdLine, "-n ") || strings.Contains(cmdLine, "--neighbors ") {
			folder += "/neighbors"
		} else if strings.Contains(cmdLine, "-C ") || strings.Contains(cmdLine, "--accounting ") {
			folder += "/accounting"
		} else if strings.Contains(cmdLine, "-A ") || strings.Contains(cmdLine, "--statements ") {
			folder += "/statements"
		} else if strings.Contains(cmdLine, "-b ") || strings.Contains(cmdLine, "--balances ") {
			folder += "/balances"
		} else {
			folder += "/transactions"
		}

	} else if parts[1] == "list" {
		folder = "./list/appearances"

	} else if parts[1] == "state" {
		folder = "./state"
		if strings.Contains(cmdLine, "-l ") || strings.Contains(cmdLine, "--call ") {
			folder += "/calls"
		} else {
			folder += "/blocks"
		}

	} else if parts[1] == "tokens" {
		folder = "./tokens"
		if strings.Contains(cmdLine, "-b ") || strings.Contains(cmdLine, "--by_acct ") {
			folder += "/by_acct"
		} else {
			folder += "/blocks"
		}
	}

	if strings.Contains(folder, "unknown") {
		return "", fmt.Errorf("unable to determine output folder for command: %s", cmdLine)
	}

	if abs, err := filepath.Abs(filepath.Join(opts.Globals.Chain, folder)); err != nil {
		return "", err
	} else {
		return abs, nil
	}
}

func (opts *MonitorsOptions) getMonitorList() []monitor.Monitor {
	var monitors []monitor.Monitor

	monitorChan := make(chan monitor.Monitor)
	go monitor.ListMonitors(opts.Globals.Chain, opts.Watchlist, monitorChan)

	for result := range monitorChan {
		switch result.Address {
		case base.NotAMonitor:
			logger.Info(fmt.Sprintf("Loaded %d monitors", len(monitors)))
			close(monitorChan)
		default:
			if result.Count() > 500000 {
				logger.Warn("Ignoring too-large address", result.Address)
				continue
			}
			monitors = append(monitors, result)
		}
	}

	return monitors
}
