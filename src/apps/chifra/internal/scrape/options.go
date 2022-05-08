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
	Sleep        float64
	Pin          bool
	BlockCnt     uint64
	Reset        uint64
	Action       string
	Publish      bool
	Blaze        bool
	BlockChanCnt uint64
	AddrChanCnt  uint64
	AppsPerChunk uint64
	UnripeDist   uint64
	SnapToGrid   uint64
	FirstSnap    uint64
	AllowMissing bool
	StartBlock   uint64
	RipeBlock    uint64
	Globals      globals.GlobalOptions
	BadFlag      error
}

var scrapeCmdLineOptions ScrapeOptions

func (opts *ScrapeOptions) TestLog() {
	logger.TestLog(len(opts.Modes) > 0, "Modes: ", opts.Modes)
	logger.TestLog(opts.Sleep != 14, "Sleep: ", opts.Sleep)
	logger.TestLog(opts.Pin, "Pin: ", opts.Pin)
	logger.TestLog(opts.BlockCnt != 2000, "BlockCnt: ", opts.BlockCnt)
	logger.TestLog(opts.Reset != utils.NOPOS, "Reset: ", opts.Reset)
	logger.TestLog(len(opts.Action) > 0, "Action: ", opts.Action)
	logger.TestLog(opts.Publish, "Publish: ", opts.Publish)
	logger.TestLog(opts.Blaze, "Blaze: ", opts.Blaze)
	logger.TestLog(opts.BlockChanCnt != 10, "BlockChanCnt: ", opts.BlockChanCnt)
	logger.TestLog(opts.AddrChanCnt != 20, "AddrChanCnt: ", opts.AddrChanCnt)
	logger.TestLog(opts.AppsPerChunk != 200000, "AppsPerChunk: ", opts.AppsPerChunk)
	logger.TestLog(opts.UnripeDist != 28, "UnripeDist: ", opts.UnripeDist)
	logger.TestLog(opts.SnapToGrid != 100000, "SnapToGrid: ", opts.SnapToGrid)
	logger.TestLog(opts.FirstSnap != 0, "FirstSnap: ", opts.FirstSnap)
	logger.TestLog(opts.AllowMissing, "AllowMissing: ", opts.AllowMissing)
	logger.TestLog(opts.StartBlock != 0, "StartBlock: ", opts.StartBlock)
	logger.TestLog(opts.RipeBlock != 0, "RipeBlock: ", opts.RipeBlock)
	opts.Globals.TestLog()
}

func (opts *ScrapeOptions) ToCmdLine() string {
	options := ""
	if opts.Pin {
		options += " --pin"
	}
	if opts.BlockCnt != 2000 {
		options += (" --block_cnt " + fmt.Sprintf("%d", opts.BlockCnt))
	}
	if opts.Reset != utils.NOPOS {
		options += (" --reset " + fmt.Sprintf("%d", opts.Reset))
	}
	if opts.Publish {
		options += " --publish"
	}
	if opts.BlockChanCnt != 10 {
		options += (" --block_chan_cnt " + fmt.Sprintf("%d", opts.BlockChanCnt))
	}
	if opts.AddrChanCnt != 20 {
		options += (" --addr_chan_cnt " + fmt.Sprintf("%d", opts.AddrChanCnt))
	}
	options += " " + strings.Join(opts.Modes, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *ScrapeOptions {
	opts := &ScrapeOptions{}
	opts.Sleep = 14
	opts.BlockCnt = 2000
	opts.Reset = utils.NOPOS
	opts.BlockChanCnt = 10
	opts.AddrChanCnt = 20
	opts.AppsPerChunk = 200000
	opts.UnripeDist = 28
	opts.SnapToGrid = 100000
	opts.FirstSnap = 0
	opts.StartBlock = 0
	opts.RipeBlock = 0
	for key, value := range r.URL.Query() {
		switch key {
		case "modes":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Modes = append(opts.Modes, s...)
			}
		case "sleep":
			opts.Sleep = globals.ToFloat64(value[0])
		case "pin":
			opts.Pin = true
		case "blockCnt":
			opts.BlockCnt = globals.ToUint64(value[0])
		case "reset":
			opts.Reset = globals.ToUint64(value[0])
		case "action":
			opts.Action = value[0]
		case "publish":
			opts.Publish = true
		case "blaze":
			opts.Blaze = true
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
		case "ripeBlock":
			opts.RipeBlock = globals.ToUint64(value[0])
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "scrape")
				return opts
			}
		}
	}
	opts.Globals = *globals.FromRequest(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

func ScrapeFinishParse(args []string) *ScrapeOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	// EXISTING_CODE
	opts.Modes = args
	// EXISTING_CODE
	return opts
}

func GetOptions() *ScrapeOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &scrapeCmdLineOptions
}
