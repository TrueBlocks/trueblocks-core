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
	// in addition to Bloom filters, download full index chunks
	All bool `json:"all,omitempty"`
	// the global options
	Globals globals.GlobalOptions `json:"globals,omitempty"`
	// an error flag if needed
	BadFlag error `json:"badFlag,omitempty"`
}

var initCmdLineOptions InitOptions

// testLog is used only during testing to export the options for this test case.
func (opts *InitOptions) testLog() {
	logger.TestLog(opts.All, "All: ", opts.All)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *InitOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
	return string(b)
}

// initFinishParseApi finishes the parsing for server invocations. Returns a new InitOptions.
func initFinishParseApi(w http.ResponseWriter, r *http.Request) *InitOptions {
	opts := &InitOptions{}
	for key, _ := range r.URL.Query() {
		switch key {
		case "all":
			opts.All = true
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
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *InitOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &initCmdLineOptions
}
