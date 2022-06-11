// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package monitorsPkg

import (
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type MonitorsOptions struct {
	Addrs      []string
	Clean      bool
	Delete     bool
	Undelete   bool
	Remove     bool
	Watch      bool
	FirstBlock uint64
	LastBlock  uint64
	Globals    globals.GlobalOptions
	BadFlag    error
}

var monitorsCmdLineOptions MonitorsOptions

func (opts *MonitorsOptions) TestLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(opts.Clean, "Clean: ", opts.Clean)
	logger.TestLog(opts.Delete, "Delete: ", opts.Delete)
	logger.TestLog(opts.Undelete, "Undelete: ", opts.Undelete)
	logger.TestLog(opts.Remove, "Remove: ", opts.Remove)
	logger.TestLog(opts.Watch, "Watch: ", opts.Watch)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS, "LastBlock: ", opts.LastBlock)
	opts.Globals.TestLog()
}

func MonitorsFinishParseApi(w http.ResponseWriter, r *http.Request) *MonitorsOptions {
	opts := &MonitorsOptions{}
	opts.FirstBlock = 0
	opts.LastBlock = utils.NOPOS
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs = append(opts.Addrs, s...)
			}
		case "clean":
			opts.Clean = true
		case "delete":
			opts.Delete = true
		case "undelete":
			opts.Undelete = true
		case "remove":
			opts.Remove = true
		case "watch":
			opts.Watch = true
		case "firstBlock":
			opts.FirstBlock = globals.ToUint64(value[0])
		case "lastBlock":
			opts.LastBlock = globals.ToUint64(value[0])
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "monitors")
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

func MonitorsFinishParse(args []string) *MonitorsOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, args)
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *MonitorsOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &monitorsCmdLineOptions
}
