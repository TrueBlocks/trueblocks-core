// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package initPkg

import (
	"fmt"
	"net/http"
	"strings"

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

func (opts *InitOptions) ToCmdLine() string {
	options := ""
	if opts.All {
		options += " --all"
	}
	options += " " + strings.Join([]string{}, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *InitOptions {
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
	opts.Globals = *globals.FromRequest(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

func InitFinishParse(args []string) *InitOptions {
	opts := GetOptions()
	// EXISTING_CODE
	// EXISTING_CODE
	return opts
}

func GetOptions() *InitOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &initCmdLineOptions
}
