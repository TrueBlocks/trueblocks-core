// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package whenPkg

import (
	"encoding/json"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// WhenOptions provides all command options for the chifra when command.
type WhenOptions struct {
	Blocks     []string                 `json:"blocks,omitempty"`     // One or more dates, block numbers, hashes, or special named blocks (see notes)
	BlockIds   []identifiers.Identifier `json:"blockIds,omitempty"`   // Block identifiers
	List       bool                     `json:"list,omitempty"`       // Export a list of the 'special' blocks
	Timestamps bool                     `json:"timestamps,omitempty"` // Ignore other options and generate timestamps only
	Check      bool                     `json:"check,omitempty"`      // Available only with --timestamps, checks the validity of the timestamp data
	Reset      uint64                   `json:"reset,omitempty"`      // Available only with --timestamps option, resets a single timestamp
	Truncate   uint64                   `json:"truncate,omitempty"`   // Available only with --timestamps option, truncates the timestamp file to this block
	Count      bool                     `json:"count,omitempty"`      // Available only with --timestamps, returns the number of timestamps in the cache
	Deep       bool                     `json:"deep,omitempty"`       // Available only with --timestamps --check option, queries every timestamp on chain (slow)
	Globals    globals.GlobalOptions    `json:"globals,omitempty"`    // The global options
	BadFlag    error                    `json:"badFlag,omitempty"`    // An error flag if needed
}

var whenCmdLineOptions WhenOptions

// testLog is used only during testing to export the options for this test case.
func (opts *WhenOptions) testLog() {
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(opts.List, "List: ", opts.List)
	logger.TestLog(opts.Timestamps, "Timestamps: ", opts.Timestamps)
	logger.TestLog(opts.Check, "Check: ", opts.Check)
	logger.TestLog(opts.Reset != utils.NOPOS, "Reset: ", opts.Reset)
	logger.TestLog(opts.Truncate != utils.NOPOS, "Truncate: ", opts.Truncate)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(opts.Deep, "Deep: ", opts.Deep)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *WhenOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
	return string(b)
}

// whenFinishParseApi finishes the parsing for server invocations. Returns a new WhenOptions.
func whenFinishParseApi(w http.ResponseWriter, r *http.Request) *WhenOptions {
	opts := &WhenOptions{}
	opts.Reset = utils.NOPOS
	opts.Truncate = utils.NOPOS
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
		case "reset":
			opts.Reset = globals.ToUint64(value[0])
		case "truncate":
			opts.Truncate = globals.ToUint64(value[0])
		case "count":
			opts.Count = true
		case "deep":
			opts.Deep = true
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

// whenFinishParse finishes the parsing for command line invocations. Returns a new WhenOptions.
func whenFinishParse(args []string) *WhenOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	opts.Blocks = args
	if opts.Truncate == 0 {
		opts.Truncate = utils.NOPOS
	}
	if opts.Reset == 0 {
		opts.Reset = utils.NOPOS
	}
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
