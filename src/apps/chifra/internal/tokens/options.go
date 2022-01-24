// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package tokensPkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type TokensOptions struct {
	Addrs2  []string
	Blocks  []string
	Parts   []string
	ByAcct  bool
	NoZero  bool
	Globals globals.GlobalOptions
	BadFlag error
}

var tokensCmdLineOptions TokensOptions

func (opts *TokensOptions) TestLog() {
	logger.TestLog(len(opts.Addrs2) > 0, "Addrs2: ", opts.Addrs2)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Parts) > 0, "Parts: ", opts.Parts)
	logger.TestLog(opts.ByAcct, "ByAcct: ", opts.ByAcct)
	logger.TestLog(opts.NoZero, "NoZero: ", opts.NoZero)
	opts.Globals.TestLog()
}

func (opts *TokensOptions) ToCmdLine() string {
	options := ""
	for _, part := range opts.Parts {
		options += " --parts " + part
	}
	if opts.ByAcct {
		options += " --by_acct"
	}
	if opts.NoZero {
		options += " --no_zero"
	}
	options += " " + strings.Join(opts.Addrs2, " ")
	options += " " + strings.Join(opts.Blocks, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *TokensOptions {
	opts := &TokensOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs2":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs2 = append(opts.Addrs2, s...)
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
		case "byAcct":
			opts.ByAcct = true
		case "noZero":
			opts.NoZero = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "tokens")
				return opts
			}
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}

func TokensFinishParse(args []string) *TokensOptions {
	opts := GetOptions()
	// EXISTING_CODE
	for _, arg := range args {
		if validate.IsValidAddress(arg) {
			opts.Addrs2 = append(opts.Addrs2, arg)
		} else {
			opts.Blocks = append(opts.Blocks, arg)
		}
	}
	// EXISTING_CODE
	return opts
}

func GetOptions() *TokensOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &tokensCmdLineOptions
}
