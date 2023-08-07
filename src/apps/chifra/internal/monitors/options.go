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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// MonitorsOptions provides all command options for the chifra monitors command.
type MonitorsOptions struct {
	Addrs    []string              `json:"addrs,omitempty"`    // One or more addresses (0x...) to process
	Delete   bool                  `json:"delete,omitempty"`   // Delete a monitor, but do not remove it
	Undelete bool                  `json:"undelete,omitempty"` // Undelete a previously deleted monitor
	Remove   bool                  `json:"remove,omitempty"`   // Remove a previously deleted monitor
	Clean    bool                  `json:"clean,omitempty"`    // Clean (i.e. remove duplicate appearances) from monitors
	List     bool                  `json:"list,omitempty"`     // List monitors in the cache (--verbose for more detail)
	Watch    bool                  `json:"watch,omitempty"`    // Continually scan for new blocks and extract data for monitored addresses
	Sleep    float64               `json:"sleep,omitempty"`    // Seconds to sleep between monitor passes
	Globals  globals.GlobalOptions `json:"globals,omitempty"`  // The global options
	Conn     *rpcClient.Connection `json:"conn,omitempty"`     // The connection to the RPC server
	BadFlag  error                 `json:"badFlag,omitempty"`  // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultMonitorsOptions = MonitorsOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *MonitorsOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(opts.Delete, "Delete: ", opts.Delete)
	logger.TestLog(opts.Undelete, "Undelete: ", opts.Undelete)
	logger.TestLog(opts.Remove, "Remove: ", opts.Remove)
	logger.TestLog(opts.Clean, "Clean: ", opts.Clean)
	logger.TestLog(opts.List, "List: ", opts.List)
	logger.TestLog(opts.Watch, "Watch: ", opts.Watch)
	logger.TestLog(opts.Sleep != float64(14), "Sleep: ", opts.Sleep)
	opts.Conn.TestLog(opts.getCaches())
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
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs = append(opts.Addrs, s...)
			}
		case "delete":
			opts.Delete = true
		case "undelete":
			opts.Undelete = true
		case "remove":
			opts.Remove = true
		case "clean":
			opts.Clean = true
		case "list":
			opts.List = true
		case "watch":
			opts.Watch = true
		case "sleep":
			opts.Sleep = globals.ToFloat64(value[0])
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "monitors")
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, r, opts.getCaches())

	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(opts.Addrs)
	// EXISTING_CODE

	return opts
}

// monitorsFinishParse finishes the parsing for command line invocations. Returns a new MonitorsOptions.
func monitorsFinishParse(args []string) *MonitorsOptions {
	defFmt := "txt"
	opts := GetOptions()
	opts.Conn = opts.Globals.FinishParse(args, opts.getCaches())

	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(args)
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
	capabilities := caps.Default // Additional global caps for chifra monitors
	// EXISTING_CODE
	// EXISTING_CODE
	defaultMonitorsOptions.Globals.Caps = capabilities
}

func (opts *MonitorsOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
