// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package initPkg

import (
	"encoding/json"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// InitOptions provides all command options for the chifra init command.
type InitOptions struct {
	All        bool                  `json:"all,omitempty"`        // In addition to Bloom filters, download full index chunks (recommended)
	DryRun     bool                  `json:"dryRun,omitempty"`     // Display the results of the download without actually downloading
	FirstBlock uint64                `json:"firstBlock,omitempty"` // Do not download any chunks earlier than this block
	Sleep      float64               `json:"sleep,omitempty"`      // Seconds to sleep between downloads
	Globals    globals.GlobalOptions `json:"globals,omitempty"`    // The global options
	BadFlag    error                 `json:"badFlag,omitempty"`    // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultInitOptions = InitOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *InitOptions) testLog() {
	logger.TestLog(opts.All, "All: ", opts.All)
	logger.TestLog(opts.DryRun, "DryRun: ", opts.DryRun)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.Sleep != float64(0.0), "Sleep: ", opts.Sleep)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *InitOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// initFinishParseApi finishes the parsing for server invocations. Returns a new InitOptions.
func initFinishParseApi(w http.ResponseWriter, r *http.Request) *InitOptions {
	copy := defaultInitOptions
	opts := &copy
	opts.FirstBlock = 0
	opts.Sleep = 0.0
	for key, value := range r.URL.Query() {
		switch key {
		case "all":
			opts.All = true
		case "dryRun":
			opts.DryRun = true
		case "firstBlock":
			opts.FirstBlock = globals.ToUint64(value[0])
		case "sleep":
			opts.Sleep = globals.ToFloat64(value[0])
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "init")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// initFinishParse finishes the parsing for command line invocations. Returns a new InitOptions.
func initFinishParse(args []string) *InitOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	if len(args) > 0 {
		opts.BadFlag = validate.Usage("Invalid argument ({0}).", args[0])
	}
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *InitOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultInitOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultInitOptions = InitOptions{}
	globals.SetDefaults(&defaultInitOptions.Globals)
	defaultInitOptions.Globals.Writer = w
}
