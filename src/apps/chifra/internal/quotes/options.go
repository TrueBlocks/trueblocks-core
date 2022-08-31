// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package quotesPkg

import (
	"encoding/json"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// QuotesOptions provides all command options for the chifra quotes command.
type QuotesOptions struct {
	Update  bool                  `json:"update,omitempty"`  // Freshen price database
	Period  string                `json:"period,omitempty"`  // Increment of display
	Pair    string                `json:"pair,omitempty"`    // Which price pair to freshen or list (see Poloniex)
	Feed    string                `json:"feed,omitempty"`    // The feed for the price data
	Globals globals.GlobalOptions `json:"globals,omitempty"` // The global options
	BadFlag error                 `json:"badFlag,omitempty"` // An error flag if needed
}

var quotesCmdLineOptions QuotesOptions

// testLog is used only during testing to export the options for this test case.
func (opts *QuotesOptions) testLog() {
	logger.TestLog(opts.Update, "Update: ", opts.Update)
	logger.TestLog(len(opts.Period) > 0, "Period: ", opts.Period)
	logger.TestLog(len(opts.Pair) > 0, "Pair: ", opts.Pair)
	logger.TestLog(len(opts.Feed) > 0, "Feed: ", opts.Feed)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *QuotesOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
	return string(b)
}

// getEnvStr allows for custom environment strings when calling to the system (helps debugging).
func (opts *QuotesOptions) getEnvStr() []string {
	envStr := []string{}
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

// toCmdLine converts the option to a command line for calling out to the system.
func (opts *QuotesOptions) toCmdLine() string {
	options := ""
	if opts.Update {
		options += " --update"
	}
	if len(opts.Period) > 0 {
		options += " --period " + opts.Period
	}
	if len(opts.Pair) > 0 {
		options += " --pair " + opts.Pair
	}
	if len(opts.Feed) > 0 {
		options += " --feed " + opts.Feed
	}
	options += " " + strings.Join([]string{}, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence compiler warning for auto gen
	return options
}

// quotesFinishParseApi finishes the parsing for server invocations. Returns a new QuotesOptions.
func quotesFinishParseApi(w http.ResponseWriter, r *http.Request) *QuotesOptions {
	opts := &QuotesOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "update":
			opts.Update = true
		case "period":
			opts.Period = value[0]
		case "pair":
			opts.Pair = value[0]
		case "feed":
			opts.Feed = value[0]
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "quotes")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// quotesFinishParse finishes the parsing for command line invocations. Returns a new QuotesOptions.
func quotesFinishParse(args []string) *QuotesOptions {
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

func GetOptions() *QuotesOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &quotesCmdLineOptions
}
