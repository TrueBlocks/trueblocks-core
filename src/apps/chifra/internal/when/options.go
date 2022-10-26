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
	Timestamps bool                     `json:"timestamps,omitempty"` // Display or process timestamps
	Count      bool                     `json:"count,omitempty"`      // With --timestamps only, returns the number of timestamps in the cache
	Truncate   uint64                   `json:"truncate,omitempty"`   // With --timestamps only, truncates the timestamp file at this block
	Repair     uint64                   `json:"repair,omitempty"`     // With --timestamps only, repair a single timestamp by querying the chain
	Check      bool                     `json:"check,omitempty"`      // With --timestamps only, checks the validity of the timestamp data
	Update     uint64                   `json:"update,omitempty"`     // With --timestamps only, bring the timestamp database forward to the latest block
	Deep       bool                     `json:"deep,omitempty"`       // With --timestamps --check only, verifies timestamps from on chain (slow)
	Globals    globals.GlobalOptions    `json:"globals,omitempty"`    // The global options
	BadFlag    error                    `json:"badFlag,omitempty"`    // An error flag if needed
}

var defaultWhenOptions = WhenOptions{
	Truncate: utils.NOPOS,
	Repair:   utils.NOPOS,
	Update:   utils.NOPOS,
}

// testLog is used only during testing to export the options for this test case.
func (opts *WhenOptions) testLog() {
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(opts.List, "List: ", opts.List)
	logger.TestLog(opts.Timestamps, "Timestamps: ", opts.Timestamps)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(opts.Truncate != utils.NOPOS, "Truncate: ", opts.Truncate)
	logger.TestLog(opts.Repair != utils.NOPOS, "Repair: ", opts.Repair)
	logger.TestLog(opts.Check, "Check: ", opts.Check)
	logger.TestLog(opts.Update != utils.NOPOS, "Update: ", opts.Update)
	logger.TestLog(opts.Deep, "Deep: ", opts.Deep)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *WhenOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// whenFinishParseApi finishes the parsing for server invocations. Returns a new WhenOptions.
func whenFinishParseApi(w http.ResponseWriter, r *http.Request) *WhenOptions {
	copy := defaultWhenOptions
	opts := &copy
	opts.Truncate = utils.NOPOS
	opts.Repair = utils.NOPOS
	opts.Update = utils.NOPOS
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
		case "count":
			opts.Count = true
		case "truncate":
			opts.Truncate = globals.ToUint64(value[0])
		case "repair":
			opts.Repair = globals.ToUint64(value[0])
		case "check":
			opts.Check = true
		case "update":
			opts.Update = globals.ToUint64(value[0])
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
	if opts.Repair == 0 {
		opts.Repair = utils.NOPOS
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
	return &defaultWhenOptions
}

func ResetOptions() {
	defaultWhenOptions = WhenOptions{}
	globals.SetDefaults(&defaultWhenOptions.Globals)
}
