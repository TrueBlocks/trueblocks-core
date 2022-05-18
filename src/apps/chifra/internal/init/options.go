// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package initPkg

import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type InitOptions struct {
	All     bool
	Globals globals.GlobalOptions
	BadFlag error
}

var initCmdLineOptions InitOptions

func (opts *InitOptions) TestLog() {
	logger.TestLog(opts.All, "All: ", opts.All)
	opts.Globals.TestLog()
}

func InitFinishParseApi(w http.ResponseWriter, r *http.Request) *InitOptions {
	opts := &InitOptions{}
	for key, _ := range r.URL.Query() {
		switch key {
		case "all":
			opts.All = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "init")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

func InitFinishParse(args []string) *InitOptions {
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

func GetOptions() *InitOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &initCmdLineOptions
}
