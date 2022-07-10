// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package monitorsPkg

import (
	"encoding/json"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// MonitorsOptions provides all command options for the chifra monitors command.
type MonitorsOptions struct {
	// one or more addresses (0x...) to process
	Addrs []string `json:"addrs,omitempty"`
	// clean (i.e. remove duplicate appearances) from monitors
	Clean bool `json:"clean,omitempty"`
	// delete a monitor, but do not remove it
	Delete bool `json:"delete,omitempty"`
	// undelete a previously deleted monitor
	Undelete bool `json:"undelete,omitempty"`
	// remove a previously deleted monitor
	Remove bool `json:"remove,omitempty"`
	// continually scan for new blocks and extract data for monitored addresses
	Watch bool `json:"watch,omitempty"`
	// seconds to sleep between monitor passes
	Sleep float64 `json:"sleep,omitempty"`
	// first block to process (inclusive)
	FirstBlock uint64 `json:"firstBlock,omitempty"`
	// last block to process (inclusive)
	LastBlock uint64 `json:"lastBlock,omitempty"`
	// the global options
	Globals globals.GlobalOptions `json:"globals,omitempty"`
	// an error flag if needed
	BadFlag error `json:"badFlag,omitempty"`
}

var monitorsCmdLineOptions MonitorsOptions

// testLog is used only during testing to export the options for this test case.
func (opts *MonitorsOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(opts.Clean, "Clean: ", opts.Clean)
	logger.TestLog(opts.Delete, "Delete: ", opts.Delete)
	logger.TestLog(opts.Undelete, "Undelete: ", opts.Undelete)
	logger.TestLog(opts.Remove, "Remove: ", opts.Remove)
	logger.TestLog(opts.Watch, "Watch: ", opts.Watch)
	logger.TestLog(opts.Sleep != 14, "Sleep: ", opts.Sleep)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS, "LastBlock: ", opts.LastBlock)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *MonitorsOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
	return string(b)
}

// monitorsFinishParseApi finishes the parsing for server invocations. Returns a new MonitorsOptions.
func monitorsFinishParseApi(w http.ResponseWriter, r *http.Request) *MonitorsOptions {
	opts := &MonitorsOptions{}
	opts.Sleep = 14
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
		case "sleep":
			opts.Sleep = globals.ToFloat64(value[0])
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

// monitorsFinishParse finishes the parsing for command line invocations. Returns a new MonitorsOptions.
func monitorsFinishParse(args []string) *MonitorsOptions {
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
