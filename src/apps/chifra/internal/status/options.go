// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package statusPkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// StatusOptions provides all command options for the chifra status command.
type StatusOptions struct {
	Mode    string                `json:"mode,omitempty"`    // The name of the binary cache to report on
	Types   []string              `json:"types,omitempty"`   // For caches mode only, which type(s) of cache to report
	Globals globals.GlobalOptions `json:"globals,omitempty"` // The global options
	BadFlag error                 `json:"badFlag,omitempty"` // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultStatusOptions = StatusOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *StatusOptions) testLog() {
	logger.TestLog(len(opts.Mode) > 0, "Mode: ", opts.Mode)
	logger.TestLog(len(opts.Types) > 0, "Types: ", opts.Types)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *StatusOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *StatusOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *StatusOptions) toCmdLine() string {
	options := ""
	for _, types := range opts.Types {
		options += " --types " + types
	}
	options += " " + opts.Mode
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence compiler warning for auto gen
	return options
}

// statusFinishParseApi finishes the parsing for server invocations. Returns a new StatusOptions.
func statusFinishParseApi(w http.ResponseWriter, r *http.Request) *StatusOptions {
	copy := defaultStatusOptions
	opts := &copy
	for key, value := range r.URL.Query() {
		switch key {
		case "mode":
			opts.Mode = value[0]
		case "types":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Types = append(opts.Types, s...)
			}
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "status")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// statusFinishParse finishes the parsing for command line invocations. Returns a new StatusOptions.
func statusFinishParse(args []string) *StatusOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	defFmt = ""
	for _, mode := range args {
		opts.Mode = mode
	}
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *StatusOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultStatusOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultStatusOptions = StatusOptions{}
	globals.SetDefaults(&defaultStatusOptions.Globals)
	defaultStatusOptions.Globals.Writer = w
}
