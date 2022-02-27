// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package explorePkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type ExploreOptions struct {
	Terms   []string
	Local   bool
	Google  bool
	Globals globals.GlobalOptions
	BadFlag error
}

var exploreCmdLineOptions ExploreOptions

func (opts *ExploreOptions) TestLog() {
	logger.TestLog(len(opts.Terms) > 0, "Terms: ", opts.Terms)
	logger.TestLog(opts.Local, "Local: ", opts.Local)
	logger.TestLog(opts.Google, "Google: ", opts.Google)
	opts.Globals.TestLog()
}

func (opts *ExploreOptions) ToCmdLine() string {
	options := ""
	if opts.Local {
		options += " --local"
	}
	if opts.Google {
		options += " --google"
	}
	options += " " + strings.Join(opts.Terms, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *ExploreOptions {
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
	opts.Globals = *globals.FromRequest(w, r)
	// EXISTING_CODE
	opts.Terms = ens.ConvertEns(opts.Globals.Chain, opts.Terms)
	// EXISTING_CODE

	return opts
}

func ExploreFinishParse(args []string) *ExploreOptions {
	opts := GetOptions()
	// EXISTING_CODE
	opts.Terms = ens.ConvertEns(opts.Globals.Chain, args)
	// EXISTING_CODE
	return opts
}

func GetOptions() *ExploreOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &exploreCmdLineOptions
}
