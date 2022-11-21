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
	Addrs      []string              `json:"addrs,omitempty"`      // One or more addresses (0x...) to process
	Clean      bool                  `json:"clean,omitempty"`      // Clean (i.e. remove duplicate appearances) from monitors
	Delete     bool                  `json:"delete,omitempty"`     // Delete a monitor, but do not remove it
	Undelete   bool                  `json:"undelete,omitempty"`   // Undelete a previously deleted monitor
	Remove     bool                  `json:"remove,omitempty"`     // Remove a previously deleted monitor
	Decache    bool                  `json:"decache,omitempty"`    // Removes a monitor and all associated data from the cache
	List       bool                  `json:"list,omitempty"`       // List monitors in the cache (--verbose for more detail)
	Watch      bool                  `json:"watch,omitempty"`      // Continually scan for new blocks and extract data for monitored addresses
	Sleep      float64               `json:"sleep,omitempty"`      // Seconds to sleep between monitor passes
	FirstBlock uint64                `json:"firstBlock,omitempty"` // First block to process (inclusive)
	LastBlock  uint64                `json:"lastBlock,omitempty"`  // Last block to process (inclusive)
	Globals    globals.GlobalOptions `json:"globals,omitempty"`    // The global options
	BadFlag    error                 `json:"badFlag,omitempty"`    // An error flag if needed
}

var defaultMonitorsOptions = MonitorsOptions{
	LastBlock: utils.NOPOS,
}

// testLog is used only during testing to export the options for this test case.
func (opts *MonitorsOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(opts.Clean, "Clean: ", opts.Clean)
	logger.TestLog(opts.Delete, "Delete: ", opts.Delete)
	logger.TestLog(opts.Undelete, "Undelete: ", opts.Undelete)
	logger.TestLog(opts.Remove, "Remove: ", opts.Remove)
	logger.TestLog(opts.Decache, "Decache: ", opts.Decache)
	logger.TestLog(opts.List, "List: ", opts.List)
	logger.TestLog(opts.Watch, "Watch: ", opts.Watch)
	logger.TestLog(opts.Sleep != float64(14), "Sleep: ", opts.Sleep)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS, "LastBlock: ", opts.LastBlock)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *MonitorsOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// monitorsFinishParseApi finishes the parsing for server invocations. Returns a new MonitorsOptions.
func monitorsFinishParseApi(w http.ResponseWriter, r *http.Request) *MonitorsOptions {
	copy := defaultMonitorsOptions
	opts := &copy
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
		case "decache":
			opts.Decache = true
		case "list":
			opts.List = true
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
	return &defaultMonitorsOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultMonitorsOptions = MonitorsOptions{}
	globals.SetDefaults(&defaultMonitorsOptions.Globals)
	defaultMonitorsOptions.Globals.Writer = w
}
