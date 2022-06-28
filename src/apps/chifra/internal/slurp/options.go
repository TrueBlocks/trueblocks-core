// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package slurpPkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type SlurpOptions struct {
	Addrs       []string
	Blocks      []string
	BlockIds    []blockRange.Identifier
	Types       []string
	Appearances bool
	Globals     globals.GlobalOptions
	BadFlag     error
}

var slurpCmdLineOptions SlurpOptions

func (opts *SlurpOptions) TestLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Types) > 0, "Types: ", opts.Types)
	logger.TestLog(opts.Appearances, "Appearances: ", opts.Appearances)
	opts.Globals.TestLog()
}

func (opts *SlurpOptions) GetEnvStr() string {
	envStr := ""
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

func (opts *SlurpOptions) ToCmdLine() string {
	options := ""
	for _, types := range opts.Types {
		options += " --types " + types
	}
	if opts.Appearances {
		options += " --appearances"
	}
	options += " " + strings.Join(opts.Addrs, " ")
	options += " " + strings.Join(opts.Blocks, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func SlurpFinishParseApi(w http.ResponseWriter, r *http.Request) *SlurpOptions {
	opts := &SlurpOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs = append(opts.Addrs, s...)
			}
		case "blocks":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Blocks = append(opts.Blocks, s...)
			}
		case "types":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Types = append(opts.Types, s...)
			}
		case "appearances":
			opts.Appearances = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "slurp")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, opts.Addrs)
	// EXISTING_CODE

	return opts
}

func SlurpFinishParse(args []string) *SlurpOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	for _, arg := range args {
		if validate.IsValidAddress(arg) {
			opts.Addrs = append(opts.Addrs, arg)
		} else {
			opts.Blocks = append(opts.Blocks, arg)
		}
	}
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, opts.Addrs)
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *SlurpOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &slurpCmdLineOptions
}
