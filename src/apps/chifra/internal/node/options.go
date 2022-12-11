// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package nodePkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// NodeOptions provides all command options for the chifra node command.
type NodeOptions struct {
	Scrape   string                `json:"scrape,omitempty"`   // Start the scraper, initialize it with either just blooms or entire index, generate for new blocks
	Monitors bool                  `json:"monitors,omitempty"` // Instruct the node to start the monitors tool
	Api      string                `json:"api,omitempty"`      // Instruct the node to start the API server
	Globals  globals.GlobalOptions `json:"globals,omitempty"`  // The global options
	BadFlag  error                 `json:"badFlag,omitempty"`  // An error flag if needed
}

var defaultNodeOptions = NodeOptions{
	Api: "on",
}

// testLog is used only during testing to export the options for this test case.
func (opts *NodeOptions) testLog() {
	logger.TestLog(len(opts.Scrape) > 0, "Scrape: ", opts.Scrape)
	logger.TestLog(opts.Monitors, "Monitors: ", opts.Monitors)
	logger.TestLog(len(opts.Api) > 0, "Api: ", opts.Api)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *NodeOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *NodeOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *NodeOptions) toCmdLine() string {
	options := ""
	options += " " + strings.Join([]string{}, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence compiler warning for auto gen
	return options
}

// nodeFinishParseApi finishes the parsing for server invocations. Returns a new NodeOptions.
func nodeFinishParseApi(w http.ResponseWriter, r *http.Request) *NodeOptions {
	copy := defaultNodeOptions
	opts := &copy
	for key, value := range r.URL.Query() {
		switch key {
		case "scrape":
			opts.Scrape = value[0]
		case "monitors":
			opts.Monitors = true
		case "api":
			opts.Api = value[0]
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "node")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// nodeFinishParse finishes the parsing for command line invocations. Returns a new NodeOptions.
func nodeFinishParse(args []string) *NodeOptions {
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

func GetOptions() *NodeOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultNodeOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultNodeOptions = NodeOptions{}
	globals.SetDefaults(&defaultNodeOptions.Globals)
	defaultNodeOptions.Globals.Writer = w
}
