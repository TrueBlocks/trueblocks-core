// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package scrapePkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type ScrapeOptions struct {
	Modes        []string
	Action       string
	Sleep        float64
	Pin          bool
	Publish      bool
	BlockCnt     uint64
	BlockChanCnt uint64
	AddrChanCnt  uint64
	AppsPerChunk uint64
	UnripeDist   uint64
	SnapToGrid   uint64
	FirstSnap    uint64
	NTestRuns    uint64
	Globals      globals.GlobalOptions
	BadFlag      error
}

func (opts *ScrapeOptions) TestLog() {
	logger.TestLog(len(opts.Modes) > 0, "Modes: ", opts.Modes)
	logger.TestLog(len(opts.Action) > 0, "Action: ", opts.Action)
	logger.TestLog(opts.Sleep != 14, "Sleep: ", opts.Sleep)
	logger.TestLog(opts.Pin, "Pin: ", opts.Pin)
	logger.TestLog(opts.Publish, "Publish: ", opts.Publish)
	logger.TestLog(opts.BlockCnt != 2000, "BlockCnt: ", opts.BlockCnt)
	logger.TestLog(opts.BlockChanCnt != 10, "BlockChanCnt: ", opts.BlockChanCnt)
	logger.TestLog(opts.AddrChanCnt != 20, "AddrChanCnt: ", opts.AddrChanCnt)
	logger.TestLog(opts.AppsPerChunk != 2000000, "AppsPerChunk: ", opts.AppsPerChunk)
	logger.TestLog(opts.UnripeDist != 28, "UnripeDist: ", opts.UnripeDist)
	logger.TestLog(opts.SnapToGrid != 100000, "SnapToGrid: ", opts.SnapToGrid)
	logger.TestLog(opts.FirstSnap != 2250000, "FirstSnap: ", opts.FirstSnap)
	logger.TestLog(opts.NTestRuns != utils.NOPOS, "NTestRuns: ", opts.NTestRuns)
	opts.Globals.TestLog()
}

func (opts *ScrapeOptions) ToCmdLine() string {
	options := ""
	if len(opts.Action) > 0 {
		options += " --action " + opts.Action
	}
	if opts.Sleep != 14 {
		options += (" --sleep " + fmt.Sprintf("%.1f", opts.Sleep))
	}
	if opts.Pin {
		options += " --pin"
	}
	if opts.Publish {
		options += " --publish"
	}
	if opts.BlockCnt != 2000 {
		options += (" --block_cnt " + fmt.Sprintf("%d", opts.BlockCnt))
	}
	if opts.BlockChanCnt != 10 {
		options += (" --block_chan_cnt " + fmt.Sprintf("%d", opts.BlockChanCnt))
	}
	if opts.AddrChanCnt != 20 {
		options += (" --addr_chan_cnt " + fmt.Sprintf("%d", opts.AddrChanCnt))
	}
	if opts.AppsPerChunk != 2000000 {
		options += (" --apps_per_chunk " + fmt.Sprintf("%d", opts.AppsPerChunk))
	}
	if opts.UnripeDist != 28 {
		options += (" --unripe_dist " + fmt.Sprintf("%d", opts.UnripeDist))
	}
	if opts.SnapToGrid != 100000 {
		options += (" --snap_to_grid " + fmt.Sprintf("%d", opts.SnapToGrid))
	}
	if opts.FirstSnap != 2250000 {
		options += (" --first_snap " + fmt.Sprintf("%d", opts.FirstSnap))
	}
	if opts.NTestRuns != utils.NOPOS {
		options += (" --n_test_runs " + fmt.Sprintf("%d", opts.NTestRuns))
	}
	options += " " + strings.Join(opts.Modes, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *ScrapeOptions {
	opts := &ScrapeOptions{}
	opts.Sleep = 14
	opts.BlockCnt = 2000
	opts.BlockChanCnt = 10
	opts.AddrChanCnt = 20
	opts.AppsPerChunk = 2000000
	opts.UnripeDist = 28
	opts.SnapToGrid = 100000
	opts.FirstSnap = 2250000
	opts.NTestRuns = utils.NOPOS
	for key, value := range r.URL.Query() {
		switch key {
		case "modes":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Modes = append(opts.Modes, s...)
			}
		case "action":
			opts.Action = value[0]
		case "sleep":
			opts.Sleep = globals.ToFloat64(value[0])
		case "pin":
			opts.Pin = true
		case "publish":
			opts.Publish = true
		case "blockCnt":
			opts.BlockCnt = globals.ToUint64(value[0])
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
		case "nTestRuns":
			opts.NTestRuns = globals.ToUint64(value[0])
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "scrape")
				return opts
			}
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}

var Options ScrapeOptions

func ScrapeFinishParse(args []string) *ScrapeOptions {
	// EXISTING_CODE
	Options.Modes = args
	// EXISTING_CODE
	return &Options
}
