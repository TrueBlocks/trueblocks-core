// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package quotesPkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type QuotesOptions struct {
	Update  bool
	Period  string
	Pair    string
	Feed    string
	Globals globals.GlobalOptions
	BadFlag error
}

var quotesCmdLineOptions QuotesOptions

func (opts *QuotesOptions) TestLog() {
	logger.TestLog(opts.Update, "Update: ", opts.Update)
	logger.TestLog(len(opts.Period) > 0, "Period: ", opts.Period)
	logger.TestLog(len(opts.Pair) > 0, "Pair: ", opts.Pair)
	logger.TestLog(len(opts.Feed) > 0, "Feed: ", opts.Feed)
	opts.Globals.TestLog()
}

func (opts *QuotesOptions) ToCmdLine() string {
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
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *QuotesOptions {
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
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}

func QuotesFinishParse(args []string) *QuotesOptions {
	opts := GetOptions()
	// EXISTING_CODE
	// EXISTING_CODE
	return opts
}

func GetOptions() *QuotesOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &quotesCmdLineOptions
}
