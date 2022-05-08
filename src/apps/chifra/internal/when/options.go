// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package whenPkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type WhenOptions struct {
	Blocks     []string
	List       bool
	Timestamps bool
	Check      bool
	Fix        bool
	Count      bool
	NoUpdate   bool
	Globals    globals.GlobalOptions
	BadFlag    error
}

var whenCmdLineOptions WhenOptions

func (opts *WhenOptions) TestLog() {
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(opts.List, "List: ", opts.List)
	logger.TestLog(opts.Timestamps, "Timestamps: ", opts.Timestamps)
	logger.TestLog(opts.Check, "Check: ", opts.Check)
	logger.TestLog(opts.Fix, "Fix: ", opts.Fix)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(opts.NoUpdate, "NoUpdate: ", opts.NoUpdate)
	opts.Globals.TestLog()
}

func (opts *WhenOptions) ToCmdLine() string {
	options := ""
	if opts.List {
		options += " --list"
	}
	if opts.Timestamps {
		options += " --timestamps"
	}
	if opts.Check {
		options += " --check"
	}
	if opts.Fix {
		options += " --fix"
	}
	if opts.Count {
		options += " --count"
	}
	if opts.NoUpdate {
		options += " --no_update"
	}
	options += " " + strings.Join(opts.Blocks, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *WhenOptions {
	opts := &WhenOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "blocks":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Blocks = append(opts.Blocks, s...)
			}
		case "list":
			opts.List = true
		case "timestamps":
			opts.Timestamps = true
		case "check":
			opts.Check = true
		case "fix":
			opts.Fix = true
		case "count":
			opts.Count = true
		case "noUpdate":
			opts.NoUpdate = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "when")
				return opts
			}
		}
	}
	opts.Globals = *globals.FromRequest(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

func WhenFinishParse(args []string) *WhenOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	// EXISTING_CODE
	opts.Blocks = args
	// EXISTING_CODE
	return opts
}

func GetOptions() *WhenOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &whenCmdLineOptions
}
