// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package pinsPkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type PinsOptions struct {
	List    bool
	Init    bool
	All     bool
	Share   bool
	Sleep   float64
	Freshen bool
	Remote  bool
	InitAll bool
	Globals globals.GlobalOptions
	BadFlag error
}

var pinsCmdLineOptions PinsOptions

func (opts *PinsOptions) TestLog() {
	logger.TestLog(opts.List, "List: ", opts.List)
	logger.TestLog(opts.Init, "Init: ", opts.Init)
	logger.TestLog(opts.All, "All: ", opts.All)
	logger.TestLog(opts.Share, "Share: ", opts.Share)
	logger.TestLog(opts.Sleep != .25, "Sleep: ", opts.Sleep)
	opts.Globals.TestLog()
}

func (opts *PinsOptions) ToCmdLine() string {
	options := ""
	if opts.List {
		options += " --list"
	}
	if opts.Init {
		options += " --init"
	}
	if opts.All {
		options += " --all"
	}
	if opts.Share {
		options += " --share"
	}
	if opts.Sleep != .25 {
		options += (" --sleep " + fmt.Sprintf("%.1f", opts.Sleep))
	}
	options += " " + strings.Join([]string{}, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *PinsOptions {
	opts := &PinsOptions{}
	opts.Sleep = .25
	for key, value := range r.URL.Query() {
		switch key {
		case "list":
			opts.List = true
		case "init":
			opts.Init = true
		case "all":
			opts.All = true
		case "share":
			opts.Share = true
		case "sleep":
			opts.Sleep = globals.ToFloat64(value[0])
		case "freshen":
			opts.Freshen = true
		case "remote":
			opts.Remote = true
		case "initAll":
			opts.InitAll = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "pins")
				return opts
			}
		}
	}
	opts.Globals = *globals.FromRequest(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

func PinsFinishParse(args []string) *PinsOptions {
	opts := GetOptions()
	// EXISTING_CODE
	// EXISTING_CODE
	return opts
}

func GetOptions() *PinsOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &pinsCmdLineOptions
}
