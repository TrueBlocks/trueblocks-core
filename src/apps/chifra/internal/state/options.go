// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package statePkg

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

type StateOptions struct {
	Addrs    []string
	Blocks   []string
	BlockIds []blockRange.Identifier
	Parts    []string
	Changes  bool
	NoZero   bool
	Call     string
	ProxyFor string
	Globals  globals.GlobalOptions
	BadFlag  error
}

var stateCmdLineOptions StateOptions

func (opts *StateOptions) TestLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Parts) > 0, "Parts: ", opts.Parts)
	logger.TestLog(opts.Changes, "Changes: ", opts.Changes)
	logger.TestLog(opts.NoZero, "NoZero: ", opts.NoZero)
	logger.TestLog(len(opts.Call) > 0, "Call: ", opts.Call)
	logger.TestLog(len(opts.ProxyFor) > 0, "ProxyFor: ", opts.ProxyFor)
	opts.Globals.TestLog()
}

func (opts *StateOptions) GetEnvStr() string {
	envStr := ""
	// EXISTING_CODE
	// EXISTING_CODE
	return envStr
}

func (opts *StateOptions) ToCmdLine() string {
	options := ""
	for _, part := range opts.Parts {
		options += " --parts " + part
	}
	if opts.Changes {
		options += " --changes"
	}
	if opts.NoZero {
		options += " --no_zero"
	}
	if len(opts.Call) > 0 {
		options += " --call " + opts.Call
	}
	if len(opts.ProxyFor) > 0 {
		options += " --proxy_for " + opts.ProxyFor
	}
	options += " " + strings.Join(opts.Addrs, " ")
	options += " " + strings.Join(opts.Blocks, " ")
	// EXISTING_CODE
	// EXISTING_CODE
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func StateFinishParseApi(w http.ResponseWriter, r *http.Request) *StateOptions {
	opts := &StateOptions{}
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
		case "parts":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Parts = append(opts.Parts, s...)
			}
		case "changes":
			opts.Changes = true
		case "noZero":
			opts.NoZero = true
		case "call":
			opts.Call = value[0]
		case "proxyFor":
			opts.ProxyFor = value[0]
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "state")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, opts.Addrs)
	opts.ProxyFor = ens.ConvertOneEns(opts.Globals.Chain, opts.ProxyFor)
	// EXISTING_CODE

	return opts
}

func StateFinishParse(args []string) *StateOptions {
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
	opts.ProxyFor = ens.ConvertOneEns(opts.Globals.Chain, opts.ProxyFor)
	opts.Call = strings.Replace(opts.Call, "|", "!", -1)
	opts.Call = strings.Replace(opts.Call, " !", "!", -1)
	opts.Call = strings.Replace(opts.Call, "! ", "!", -1)
	parts := strings.Split(opts.Call, "!")
	if len(parts) > 0 {
		opts.Call = strings.Replace(opts.Call, parts[0], ens.ConvertOneEns(opts.Globals.Chain, parts[0]), -1)
	}
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *StateOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &stateCmdLineOptions
}
