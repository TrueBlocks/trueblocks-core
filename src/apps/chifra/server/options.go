// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package servePkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// ServeOptions provides all command options for the chifra serve command.
type ServeOptions struct {
	Port    string                `json:"port,omitempty"`    // Specify the server's port
	Globals globals.GlobalOptions `json:"globals,omitempty"` // The global options
	BadFlag error                 `json:"badFlag,omitempty"` // An error flag if needed
}

var defaultServeOptions = ServeOptions{
	Port: ":8080",
}

// testLog is used only during testing to export the options for this test case.
func (opts *ServeOptions) testLog() {
	logger.TestLog(len(opts.Port) > 0, "Port: ", opts.Port)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ServeOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *ServeOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *ServeOptions) toCmdLine() string {
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

// serveFinishParseApi finishes the parsing for server invocations. Returns a new ServeOptions.
func serveFinishParseApi(w http.ResponseWriter, r *http.Request) *ServeOptions {
	copy := defaultServeOptions
	opts := &copy
	for key, value := range r.URL.Query() {
		switch key {
		case "port":
			opts.Port = value[0]
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "serve")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// serveFinishParse finishes the parsing for command line invocations. Returns a new ServeOptions.
func serveFinishParse(args []string) *ServeOptions {
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

func GetOptions() *ServeOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultServeOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls if
	// the global format is JSON
	w := GetOptions().Globals.Writer
	defaultServeOptions = ServeOptions{}
	globals.SetDefaults(&defaultServeOptions.Globals)
	if GetOptions().Globals.Format == "json" || GetOptions().Globals.ShowRaw {
		// Preserve writer
		defaultServeOptions.Globals.Writer = w
	} else {
		// Reset writer to the default one
		defaultServeOptions.Globals.Writer = os.Stdout
	}
}
