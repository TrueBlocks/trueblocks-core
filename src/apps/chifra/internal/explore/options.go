// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package explorePkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// ExploreOptions provides all command options for the chifra explore command.
type ExploreOptions struct {
	Terms   []string
	Local   bool
	Google  bool
	Globals globals.GlobalOptions
	BadFlag error
}

var exploreCmdLineOptions ExploreOptions

// TestLog is used only during testing to export the options for this test case.
func (opts *ExploreOptions) TestLog() {
	logger.TestLog(len(opts.Terms) > 0, "Terms: ", opts.Terms)
	logger.TestLog(opts.Local, "Local: ", opts.Local)
	logger.TestLog(opts.Google, "Google: ", opts.Google)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ExploreOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
	return string(b)
}

// getEnvStr allows for adding custom environment strings when calling out to the command line (useful for debugging).
func (opts *ExploreOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the options object to a command line for calling out to the system.
func (opts *ExploreOptions) toCmdLine() string {
	options := ""
	if opts.Local {
		options += " --local"
	}
	if opts.Google {
		options += " --google"
	}
	options += " " + strings.Join(opts.Terms, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

// exploreFinishParseApi finishes the parsing for server invocations. Returns a new ExploreOptions.
func exploreFinishParseApi(w http.ResponseWriter, r *http.Request) *ExploreOptions {
	opts := &ExploreOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "terms":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Terms = append(opts.Terms, s...)
			}
		case "local":
			opts.Local = true
		case "google":
			opts.Google = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "explore")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	opts.Terms = ens.ConvertEns(opts.Globals.Chain, opts.Terms)
	// EXISTING_CODE

	return opts
}

// exploreFinishParse finishes the parsing for command line invocations. Returns a new ExploreOptions.
func exploreFinishParse(args []string) *ExploreOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	opts.Terms = ens.ConvertEns(opts.Globals.Chain, args)
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *ExploreOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &exploreCmdLineOptions
}
