// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package monitorsPkg

import (
	// EXISTING_CODE
	"encoding/json"
	"io"
	"net/http"
	"net/url"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	// EXISTING_CODE
)

// MonitorsOptions provides all command options for the chifra monitors command.
type MonitorsOptions struct {
	Addrs     []string              `json:"addrs,omitempty"`     // One or more addresses (0x...) to process
	Delete    bool                  `json:"delete,omitempty"`    // Delete a monitor, but do not remove it
	Undelete  bool                  `json:"undelete,omitempty"`  // Undelete a previously deleted monitor
	Remove    bool                  `json:"remove,omitempty"`    // Remove a previously deleted monitor
	Clean     bool                  `json:"clean,omitempty"`     // Clean (i.e. remove duplicate appearances) from monitors
	List      bool                  `json:"list,omitempty"`      // List monitors in the cache (--verbose for more detail)
	Watch     bool                  `json:"watch,omitempty"`     // Continually scan for new blocks and extract data as per the command file
	Watchlist string                `json:"watchlist,omitempty"` // Available with --watch option only, a file containing the addresses to watch
	Commands  string                `json:"commands,omitempty"`  // Available with --watch option only, the file containing the list of commands to apply to each watched address
	BatchSize uint64                `json:"batchSize,omitempty"` // Available with --watch option only, the number of monitors to process in each batch
	RunCount  uint64                `json:"runCount,omitempty"`  // Available with --watch option only, run the monitor this many times, then quit
	Sleep     float64               `json:"sleep,omitempty"`     // Available with --watch option only, the number of seconds to sleep between runs
	Globals   globals.GlobalOptions `json:"globals,omitempty"`   // The global options
	Conn      *rpc.Connection       `json:"conn,omitempty"`      // The connection to the RPC server
	BadFlag   error                 `json:"badFlag,omitempty"`   // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultMonitorsOptions = MonitorsOptions{
	BatchSize: 8,
	Sleep:     14,
}

// testLog is used only during testing to export the options for this test case.
func (opts *MonitorsOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(opts.Delete, "Delete: ", opts.Delete)
	logger.TestLog(opts.Undelete, "Undelete: ", opts.Undelete)
	logger.TestLog(opts.Remove, "Remove: ", opts.Remove)
	logger.TestLog(opts.Clean, "Clean: ", opts.Clean)
	logger.TestLog(opts.List, "List: ", opts.List)
	logger.TestLog(opts.Watch, "Watch: ", opts.Watch)
	logger.TestLog(len(opts.Watchlist) > 0, "Watchlist: ", opts.Watchlist)
	logger.TestLog(len(opts.Commands) > 0, "Commands: ", opts.Commands)
	logger.TestLog(opts.BatchSize != 8, "BatchSize: ", opts.BatchSize)
	logger.TestLog(opts.RunCount != 0, "RunCount: ", opts.RunCount)
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
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return MonitorsFinishParseInternal(w, values)
}

func MonitorsFinishParseInternal(w io.Writer, values url.Values) *MonitorsOptions {
	copy := defaultMonitorsOptions
	copy.Globals.Caps = getCaps()
	opts := &copy
	opts.BatchSize = 8
	opts.Sleep = 14
	for key, value := range values {
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
		case "watchlist":
			opts.Watchlist = value[0]
		case "commands":
			opts.Commands = value[0]
		case "batchSize":
			opts.BatchSize = base.MustParseUint(value[0])
		case "runCount":
			opts.RunCount = base.MustParseUint(value[0])
		case "sleep":
			opts.Sleep = base.MustParseFloat(value[0])
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "monitors")
				if opts.BadFlag == nil || opts.BadFlag.Error() > err.Error() {
					opts.BadFlag = err
				}
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())

	// EXISTING_CODE
	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(opts.Addrs)

	return opts
}

// monitorsFinishParse finishes the parsing for command line invocations. Returns a new MonitorsOptions.
func monitorsFinishParse(args []string) *MonitorsOptions {
	// remove duplicates from args if any (not needed in api mode because the server does it).
	dedup := map[string]int{}
	if len(args) > 0 {
		tmp := []string{}
		for _, arg := range args {
			if value := dedup[arg]; value == 0 {
				tmp = append(tmp, arg)
			}
			dedup[arg]++
		}
		args = tmp
	}

	defFmt := "txt"
	opts := GetOptions()
	opts.Conn = opts.Globals.FinishParse(args, opts.getCaches())

	// EXISTING_CODE
	for _, arg := range args {
		if base.IsValidAddress(arg) {
			opts.Addrs = append(opts.Addrs, arg)
		}
	}
	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(opts.Addrs)
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

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra monitors
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultMonitorsOptions = MonitorsOptions{}
	globals.SetDefaults(&defaultMonitorsOptions.Globals)
	defaultMonitorsOptions.Globals.TestMode = testMode
	defaultMonitorsOptions.Globals.Writer = w
	// EXISTING_CODE
	// EXISTING_CODE
	defaultMonitorsOptions.Globals.Caps = getCaps()
}

func (opts *MonitorsOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
