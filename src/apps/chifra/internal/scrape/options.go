// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package scrapePkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// ScrapeOptions provides all command options for the chifra scrape command.
type ScrapeOptions struct {
	Modes        []string              `json:"modes,omitempty"`        // Which scraper(s) to control
	BlockCnt     uint64                `json:"blockCnt,omitempty"`     // Maximum number of blocks to process per pass
	Pin          bool                  `json:"pin,omitempty"`          // Pin chunks (and blooms) to IPFS as they are created (requires ipfs daemon)
	Sleep        float64               `json:"sleep,omitempty"`        // Seconds to sleep between scraper passes
	BlockChanCnt uint64                `json:"blockChanCnt,omitempty"` // Number of concurrent block processing channels
	AddrChanCnt  uint64                `json:"addrChanCnt,omitempty"`  // Number of concurrent address processing channels
	AppsPerChunk uint64                `json:"appsPerChunk,omitempty"` // The number of appearances to build into a chunk before consolidating it
	UnripeDist   uint64                `json:"unripeDist,omitempty"`   // The distance (in blocks) from the front of the chain under which (inclusive) a block is considered unripe
	SnapToGrid   uint64                `json:"snapToGrid,omitempty"`   // An override to apps_per_chunk to snap-to-grid at every modulo of this value, this allows easier corrections to the index
	FirstSnap    uint64                `json:"firstSnap,omitempty"`    // The first block at which snap_to_grid is enabled
	AllowMissing bool                  `json:"allowMissing,omitempty"` // Do not report errors for blockchain that contain blocks with zero addresses
	StartBlock   uint64                `json:"startBlock,omitempty"`   // First block to visit (available only for blaze scraper)
	Globals      globals.GlobalOptions `json:"globals,omitempty"`      // The global options
	BadFlag      error                 `json:"badFlag,omitempty"`      // An error flag if needed
}

var scrapeCmdLineOptions ScrapeOptions

// testLog is used only during testing to export the options for this test case.
func (opts *ScrapeOptions) testLog() {
	logger.TestLog(len(opts.Modes) > 0, "Modes: ", opts.Modes)
	logger.TestLog(opts.BlockCnt != 2000, "BlockCnt: ", opts.BlockCnt)
	logger.TestLog(opts.Pin, "Pin: ", opts.Pin)
	logger.TestLog(opts.Sleep != 14, "Sleep: ", opts.Sleep)
	logger.TestLog(opts.BlockChanCnt != 10, "BlockChanCnt: ", opts.BlockChanCnt)
	logger.TestLog(opts.AddrChanCnt != 20, "AddrChanCnt: ", opts.AddrChanCnt)
	logger.TestLog(opts.AppsPerChunk != 200000, "AppsPerChunk: ", opts.AppsPerChunk)
	logger.TestLog(opts.UnripeDist != 28, "UnripeDist: ", opts.UnripeDist)
	logger.TestLog(opts.SnapToGrid != 100000, "SnapToGrid: ", opts.SnapToGrid)
	logger.TestLog(opts.FirstSnap != 0, "FirstSnap: ", opts.FirstSnap)
	logger.TestLog(opts.AllowMissing, "AllowMissing: ", opts.AllowMissing)
	logger.TestLog(opts.StartBlock != 0, "StartBlock: ", opts.StartBlock)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ScrapeOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *ScrapeOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *ScrapeOptions) toCmdLine() string {
	options := ""
	options += " " + strings.Join(opts.Modes, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

// scrapeFinishParseApi finishes the parsing for server invocations. Returns a new ScrapeOptions.
func scrapeFinishParseApi(w http.ResponseWriter, r *http.Request) *ScrapeOptions {
	opts := &ScrapeOptions{}
	opts.BlockCnt = 2000
	opts.Sleep = 14
	opts.BlockChanCnt = 10
	opts.AddrChanCnt = 20
	opts.AppsPerChunk = 200000
	opts.UnripeDist = 28
	opts.SnapToGrid = 100000
	opts.FirstSnap = 0
	opts.StartBlock = 0
	for key, value := range r.URL.Query() {
		switch key {
		case "modes":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Modes = append(opts.Modes, s...)
			}
		case "blockCnt":
			opts.BlockCnt = globals.ToUint64(value[0])
		case "pin":
			opts.Pin = true
		case "sleep":
			opts.Sleep = globals.ToFloat64(value[0])
		case "blockChanCnt":
			opts.BlockChanCnt = globals.ToUint64(value[0])
		case "addrChanCnt":
			opts.AddrChanCnt = globals.ToUint64(value[0])
		case "appsPerChunk":
			opts.AppsPerChunk = globals.ToUint64(value[0])
		case "unripeDist":
			opts.UnripeDist = globals.ToUint64(value[0])
		case "snapToGrid":
			opts.SnapToGrid = globals.ToUint64(value[0])
		case "firstSnap":
			opts.FirstSnap = globals.ToUint64(value[0])
		case "allowMissing":
			opts.AllowMissing = true
		case "startBlock":
			opts.StartBlock = globals.ToUint64(value[0])
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "scrape")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// scrapeFinishParse finishes the parsing for command line invocations. Returns a new ScrapeOptions.
func scrapeFinishParse(args []string) *ScrapeOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	opts.Modes = args
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *ScrapeOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &scrapeCmdLineOptions
}
