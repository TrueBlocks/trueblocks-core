// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package whenPkg

import (
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type WhenOptions struct {
	Blocks     []string
	BlockNums  []blockRange.BlockRange
	List       bool
	Timestamps bool
	Check      bool
	Count      bool
	Globals    globals.GlobalOptions
	BadFlag    error
}

var whenCmdLineOptions WhenOptions

func (opts *WhenOptions) TestLog() {
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(opts.List, "List: ", opts.List)
	logger.TestLog(opts.Timestamps, "Timestamps: ", opts.Timestamps)
	logger.TestLog(opts.Check, "Check: ", opts.Check)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	opts.Globals.TestLog()
}

func WhenFinishParseApi(w http.ResponseWriter, r *http.Request) *WhenOptions {
	opts := &WhenOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "blocks":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Blocks = append(opts.Blocks, s...)
			}
		case "list":
			opts.List = true
		case "timestamps":
			opts.Timestamps = true
		case "check":
			opts.Check = true
		case "count":
			opts.Count = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "when")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

func WhenFinishParse(args []string) *WhenOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	opts.Blocks = args
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *WhenOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &whenCmdLineOptions
}
