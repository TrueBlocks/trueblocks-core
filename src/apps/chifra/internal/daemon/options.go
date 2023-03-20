// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package daemonPkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// DaemonOptions provides all command options for the chifra daemon command.
type DaemonOptions struct {
	Port    string                `json:"port,omitempty"`    // Specify the server's port
	Api     string                `json:"api,omitempty"`     // Instruct the node to start the API server
	Scrape  string                `json:"scrape,omitempty"`  // Start the scraper, initialize it with either just blooms or entire index, generate for new blocks
	Monitor bool                  `json:"monitor,omitempty"` // Instruct the node to start the monitors tool
	Globals globals.GlobalOptions `json:"globals,omitempty"` // The global options
	BadFlag error                 `json:"badFlag,omitempty"` // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultDaemonOptions = DaemonOptions{
	Port: ":8080",
	Api:  "on",
}

// testLog is used only during testing to export the options for this test case.
func (opts *DaemonOptions) testLog() {
	logger.TestLog(len(opts.Port) > 0, "Port: ", opts.Port)
	logger.TestLog(len(opts.Api) > 0, "Api: ", opts.Api)
	logger.TestLog(len(opts.Scrape) > 0, "Scrape: ", opts.Scrape)
	logger.TestLog(opts.Monitor, "Monitor: ", opts.Monitor)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *DaemonOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *DaemonOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *DaemonOptions) toCmdLine() string {
	options := ""
	if len(opts.Port) > 0 {
		options += " --port " + opts.Port
	}
	options += " " + strings.Join([]string{}, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence compiler warning for auto gen
	return options
}

// daemonFinishParseApi finishes the parsing for server invocations. Returns a new DaemonOptions.
func daemonFinishParseApi(w http.ResponseWriter, r *http.Request) *DaemonOptions {
	copy := defaultDaemonOptions
	opts := &copy
	for key, value := range r.URL.Query() {
		switch key {
		case "port":
			opts.Port = value[0]
		case "api":
			opts.Api = value[0]
		case "scrape":
			opts.Scrape = value[0]
		case "monitor":
			opts.Monitor = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "daemon")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// daemonFinishParse finishes the parsing for command line invocations. Returns a new DaemonOptions.
func daemonFinishParse(args []string) *DaemonOptions {
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

func GetOptions() *DaemonOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultDaemonOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultDaemonOptions = DaemonOptions{}
	globals.SetDefaults(&defaultDaemonOptions.Globals)
	defaultDaemonOptions.Globals.Writer = w
}
