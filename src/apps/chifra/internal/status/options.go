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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// StatusOptions provides all command options for the chifra status command.
type StatusOptions struct {
	Modes      []string
	Details    bool
	Types      []string
	Depth      uint64
	Report     bool
	Terse      bool
	Migrate    string
	FirstBlock uint64
	LastBlock  uint64
	Globals    globals.GlobalOptions
	BadFlag    error
}

var statusCmdLineOptions StatusOptions

// testLog is used only during testing to export the options for this test case.
func (opts *StatusOptions) testLog() {
	logger.TestLog(len(opts.Modes) > 0, "Modes: ", opts.Modes)
	logger.TestLog(opts.Details, "Details: ", opts.Details)
	logger.TestLog(len(opts.Types) > 0, "Types: ", opts.Types)
	logger.TestLog(opts.Depth != utils.NOPOS, "Depth: ", opts.Depth)
	logger.TestLog(opts.Terse, "Terse: ", opts.Terse)
	logger.TestLog(len(opts.Migrate) > 0, "Migrate: ", opts.Migrate)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS, "LastBlock: ", opts.LastBlock)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *StatusOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
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
	if opts.Details {
		options += " --details"
	}
	for _, types := range opts.Types {
		options += " --types " + types
	}
	if opts.Depth != utils.NOPOS {
		options += (" --depth " + fmt.Sprintf("%d", opts.Depth))
	}
	if opts.Terse {
		options += " --terse"
	}
	if len(opts.Migrate) > 0 {
		options += " --migrate " + opts.Migrate
	}
	if opts.FirstBlock != 0 {
		options += (" --first_block " + fmt.Sprintf("%d", opts.FirstBlock))
	}
	if opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS {
		options += (" --last_block " + fmt.Sprintf("%d", opts.LastBlock))
	}
	options += " " + strings.Join(opts.Modes, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

// statusFinishParseApi finishes the parsing for server invocations. Returns a new StatusOptions.
func statusFinishParseApi(w http.ResponseWriter, r *http.Request) *StatusOptions {
	opts := &StatusOptions{}
	opts.Depth = utils.NOPOS
	opts.FirstBlock = 0
	opts.LastBlock = utils.NOPOS
	for key, value := range r.URL.Query() {
		switch key {
		case "modes":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Modes = append(opts.Modes, s...)
			}
		case "details":
			opts.Details = true
		case "types":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Types = append(opts.Types, s...)
			}
		case "depth":
			opts.Depth = globals.ToUint64(value[0])
		case "report":
			opts.Report = true
		case "terse":
			opts.Terse = true
		case "migrate":
			opts.Migrate = value[0]
		case "firstBlock":
			opts.FirstBlock = globals.ToUint64(value[0])
		case "lastBlock":
			opts.LastBlock = globals.ToUint64(value[0])
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
	opts.Modes = args
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *StatusOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &statusCmdLineOptions
}
