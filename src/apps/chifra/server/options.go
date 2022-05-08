// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package servePkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type ServeOptions struct {
	Port    string
	Globals globals.GlobalOptions
	BadFlag error
}

var serveCmdLineOptions ServeOptions

func (opts *ServeOptions) TestLog() {
	logger.TestLog(len(opts.Port) > 0, "Port: ", opts.Port)
	opts.Globals.TestLog()
}

func (opts *ServeOptions) ToCmdLine() string {
	options := ""
	if len(opts.Port) > 0 {
		options += " --port " + opts.Port
	}
	options += " " + strings.Join([]string{}, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *ServeOptions {
	opts := &ServeOptions{}
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
	opts.Globals = *globals.FromRequest(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

func ServeFinishParse(args []string) *ServeOptions {
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
	return &serveCmdLineOptions
}
