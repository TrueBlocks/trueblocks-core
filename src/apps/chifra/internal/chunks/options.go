// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package chunksPkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type ChunksOptions struct {
	Blocks  []string
	Extract string
	Globals globals.GlobalOptions
	BadFlag error
}

var chunksCmdLineOptions ChunksOptions

func (opts *ChunksOptions) TestLog() {
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Extract) > 0, "Extract: ", opts.Extract)
	opts.Globals.TestLog()
}

func (opts *ChunksOptions) ToCmdLine() string {
	options := ""
	if len(opts.Extract) > 0 {
		options += " --extract " + opts.Extract
	}
	options += " " + strings.Join(opts.Blocks, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *ChunksOptions {
	opts := &ChunksOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "blocks":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Blocks = append(opts.Blocks, s...)
			}
		case "extract":
			opts.Extract = value[0]
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "chunks")
				return opts
			}
		}
	}
	opts.Globals = *globals.FromRequest(w, r)
	// EXISTING_CODE
	opts.Extract = ens.ConvertOneEns(opts.Globals.Chain, opts.Extract)
	// EXISTING_CODE

	return opts
}

func ChunksFinishParse(args []string) *ChunksOptions {
	opts := GetOptions()
	// EXISTING_CODE
	opts.Blocks = args
	opts.Extract = ens.ConvertOneEns(opts.Globals.Chain, opts.Extract)
	// EXISTING_CODE
	return opts
}

func GetOptions() *ChunksOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &chunksCmdLineOptions
}
