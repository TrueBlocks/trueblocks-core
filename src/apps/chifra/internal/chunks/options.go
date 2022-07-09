// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package chunksPkg

import (
	"encoding/json"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type ChunksOptions struct {
	Mode      string
	Blocks    []string
	BlockIds  []identifiers.Identifier
	Addrs     []string
	Details   bool
	Check     bool
	Belongs   bool
	Repair    bool
	Clean     bool
	Remote    bool
	Reset     uint64
	PinRemote bool
	Publish   bool
	Globals   globals.GlobalOptions
	BadFlag   error
}

var chunksCmdLineOptions ChunksOptions

func (opts *ChunksOptions) TestLog() {
	logger.TestLog(len(opts.Mode) > 0, "Mode: ", opts.Mode)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(opts.Details, "Details: ", opts.Details)
	logger.TestLog(opts.Check, "Check: ", opts.Check)
	logger.TestLog(opts.Belongs, "Belongs: ", opts.Belongs)
	logger.TestLog(opts.Repair, "Repair: ", opts.Repair)
	logger.TestLog(opts.Clean, "Clean: ", opts.Clean)
	logger.TestLog(opts.Remote, "Remote: ", opts.Remote)
	logger.TestLog(opts.Reset != utils.NOPOS, "Reset: ", opts.Reset)
	logger.TestLog(opts.PinRemote, "PinRemote: ", opts.PinRemote)
	logger.TestLog(opts.Publish, "Publish: ", opts.Publish)
	opts.Globals.TestLog()
}

func (opts *ChunksOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
	return string(b)
}

func ChunksFinishParseApi(w http.ResponseWriter, r *http.Request) *ChunksOptions {
	opts := &ChunksOptions{}
	opts.Reset = utils.NOPOS
	for key, value := range r.URL.Query() {
		switch key {
		case "mode":
			opts.Mode = value[0]
		case "blocks":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Blocks = append(opts.Blocks, s...)
			}
		case "addrs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs = append(opts.Addrs, s...)
			}
		case "details":
			opts.Details = true
		case "check":
			opts.Check = true
		case "belongs":
			opts.Belongs = true
		case "repair":
			opts.Repair = true
		case "clean":
			opts.Clean = true
		case "remote":
			opts.Remote = true
		case "reset":
			opts.Reset = globals.ToUint64(value[0])
		case "pinRemote":
			opts.PinRemote = true
		case "publish":
			opts.Publish = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "chunks")
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

func ChunksFinishParse(args []string) *ChunksOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	if len(args) > 0 {
		opts.Mode = args[0]
		for i, arg := range args {
			if i > 0 {
				if validate.IsValidAddress(arg) {
					opts.Addrs = append(opts.Addrs, arg)
				} else {
					opts.Blocks = append(opts.Blocks, arg)
				}
			}
		}
	}
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, opts.Addrs)
	if opts.Mode == "manifest" {
		defFmt = "json"
	}
	if opts.Reset == 0 {
		opts.Reset = utils.NOPOS
	}
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *ChunksOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &chunksCmdLineOptions
}
