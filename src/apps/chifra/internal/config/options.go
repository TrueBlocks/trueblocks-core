// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package configPkg

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

// ConfigOptions provides all command options for the chifra config command.
type ConfigOptions struct {
	Modes      []string              `json:"modes,omitempty"`      // Either show or edit the configuration
	Module     []string              `json:"module,omitempty"`     // The type of information to show or edit
	Details    bool                  `json:"details,omitempty"`    // Include details about items found in monitors, slurps, abis, or price caches
	Types      []string              `json:"types,omitempty"`      // For caches module only, which type(s) of cache to report
	Depth      uint64                `json:"depth,omitempty"`      // For caches module only, number of levels deep to report
	Terse      bool                  `json:"terse,omitempty"`      // Show a terse summary report for mode show
	Paths      bool                  `json:"paths,omitempty"`      // Show the configuration paths for the system
	FirstBlock uint64                `json:"firstBlock,omitempty"` // First block to process (inclusive -- testing only)
	LastBlock  uint64                `json:"lastBlock,omitempty"`  // Last block to process (inclusive -- testing only)
	Globals    globals.GlobalOptions `json:"globals,omitempty"`    // The global options
	BadFlag    error                 `json:"badFlag,omitempty"`    // An error flag if needed
}

var defaultConfigOptions = ConfigOptions{
	Depth:     utils.NOPOS,
	LastBlock: utils.NOPOS,
}

// testLog is used only during testing to export the options for this test case.
func (opts *ConfigOptions) testLog() {
	logger.TestLog(len(opts.Modes) > 0, "Modes: ", opts.Modes)
	logger.TestLog(len(opts.Module) > 0, "Module: ", opts.Module)
	logger.TestLog(opts.Details, "Details: ", opts.Details)
	logger.TestLog(len(opts.Types) > 0, "Types: ", opts.Types)
	logger.TestLog(opts.Depth != utils.NOPOS, "Depth: ", opts.Depth)
	logger.TestLog(opts.Terse, "Terse: ", opts.Terse)
	logger.TestLog(opts.Paths, "Paths: ", opts.Paths)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS, "LastBlock: ", opts.LastBlock)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ConfigOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *ConfigOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *ConfigOptions) toCmdLine() string {
	options := ""
	for _, module := range opts.Module {
		options += " --module " + module
	}
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
	if opts.FirstBlock != 0 {
		options += (" --first_block " + fmt.Sprintf("%d", opts.FirstBlock))
	}
	if opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS {
		options += (" --last_block " + fmt.Sprintf("%d", opts.LastBlock))
	}
	options += " " + strings.Join(opts.Modes, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence compiler warning for auto gen
	return options
}

// configFinishParseApi finishes the parsing for server invocations. Returns a new ConfigOptions.
func configFinishParseApi(w http.ResponseWriter, r *http.Request) *ConfigOptions {
	copy := defaultConfigOptions
	opts := &copy
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
		case "module":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Module = append(opts.Module, s...)
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
		case "terse":
			opts.Terse = true
		case "paths":
			opts.Paths = true
		case "firstBlock":
			opts.FirstBlock = globals.ToUint64(value[0])
		case "lastBlock":
			opts.LastBlock = globals.ToUint64(value[0])
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "config")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// configFinishParse finishes the parsing for command line invocations. Returns a new ConfigOptions.
func configFinishParse(args []string) *ConfigOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	defFmt = ""
	for _, mode := range args {
		if mode == "show" || mode == "edit" {
			opts.Modes = append(opts.Modes, mode)
		} else {
			opts.Module = append(opts.Module, mode)
		}
	}
	if len(opts.Modes) == 0 {
		opts.Modes = []string{"show"}
		opts.Terse = true
	}
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *ConfigOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultConfigOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultConfigOptions = ConfigOptions{}
	globals.SetDefaults(&defaultConfigOptions.Globals)
	defaultConfigOptions.Globals.Writer = w
}
