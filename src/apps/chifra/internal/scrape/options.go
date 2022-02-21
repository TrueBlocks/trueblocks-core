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
	Globals      globals.GlobalOptions
	BadFlag      error
}

var scrapeCmdLineOptions ScrapeOptions

func (opts *ScrapeOptions) TestLog() {
	logger.TestLog(len(opts.Modes) > 0, "Modes: ", opts.Modes)
	logger.TestLog(len(opts.Action) > 0, "Action: ", opts.Action)
	logger.TestLog(opts.Sleep != 14, "Sleep: ", opts.Sleep)
	logger.TestLog(opts.Pin, "Pin: ", opts.Pin)
	logger.TestLog(opts.Publish, "Publish: ", opts.Publish)
	logger.TestLog(opts.BlockCnt != 2000, "BlockCnt: ", opts.BlockCnt)
	logger.TestLog(opts.BlockChanCnt != 10, "BlockChanCnt: ", opts.BlockChanCnt)
	logger.TestLog(opts.AddrChanCnt != 20, "AddrChanCnt: ", opts.AddrChanCnt)
	opts.Globals.TestLog()
}

func (opts *ScrapeOptions) ToCmdLine() string {
	options := ""
	if len(opts.Action) > 0 {
		options += " --action " + opts.Action
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
