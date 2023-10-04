// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package listPkg

import (
	"encoding/json"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// ListOptions provides all command options for the chifra list command.
type ListOptions struct {
	Addrs       []string              `json:"addrs,omitempty"`       // One or more addresses (0x...) to list
	Count       bool                  `json:"count,omitempty"`       // Display only the count of records for each monitor
	NoZero      bool                  `json:"noZero,omitempty"`      // For the --count option only, suppress the display of zero appearance accounts
	Bounds      bool                  `json:"bounds,omitempty"`      // Report first and last block this address appears
	Unripe      bool                  `json:"unripe,omitempty"`      // List transactions labeled upripe (i.e. less than 28 blocks old)
	Silent      bool                  `json:"silent,omitempty"`      // Freshen the monitor only (no reporting)
	FirstRecord uint64                `json:"firstRecord,omitempty"` // The first record to process
	MaxRecords  uint64                `json:"maxRecords,omitempty"`  // The maximum number of records to process
	Reversed    bool                  `json:"reversed,omitempty"`    // Produce results in reverse chronological order
	Publisher   string                `json:"publisher,omitempty"`   // For some query options, the publisher of the index
	FirstBlock  uint64                `json:"firstBlock,omitempty"`  // First block to export (inclusive, ignored when freshening)
	LastBlock   uint64                `json:"lastBlock,omitempty"`   // Last block to export (inclusive, ignored when freshening)
	Globals     globals.GlobalOptions `json:"globals,omitempty"`     // The global options
	Conn        *rpc.Connection       `json:"conn,omitempty"`        // The connection to the RPC server
	BadFlag     error                 `json:"badFlag,omitempty"`     // An error flag if needed
	// EXISTING_CODE
	PublisherAddr base.Address `json:"-"`
	// EXISTING_CODE
}

var defaultListOptions = ListOptions{
	MaxRecords: 250,
	Publisher:  "trueblocks.eth",
	LastBlock:  utils.NOPOS,
}

// testLog is used only during testing to export the options for this test case.
func (opts *ListOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(opts.NoZero, "NoZero: ", opts.NoZero)
	logger.TestLog(opts.Bounds, "Bounds: ", opts.Bounds)
	logger.TestLog(opts.Unripe, "Unripe: ", opts.Unripe)
	logger.TestLog(opts.Silent, "Silent: ", opts.Silent)
	logger.TestLog(opts.FirstRecord != 0, "FirstRecord: ", opts.FirstRecord)
	logger.TestLog(opts.MaxRecords != 250, "MaxRecords: ", opts.MaxRecords)
	logger.TestLog(opts.Reversed, "Reversed: ", opts.Reversed)
	logger.TestLog(!rpc.IsSame(opts.Publisher, "trueblocks.eth"), "Publisher: ", opts.Publisher)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS, "LastBlock: ", opts.LastBlock)
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ListOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// listFinishParseApi finishes the parsing for server invocations. Returns a new ListOptions.
func listFinishParseApi(w http.ResponseWriter, r *http.Request) *ListOptions {
	copy := defaultListOptions
	opts := &copy
	opts.FirstRecord = 0
	opts.MaxRecords = 250
	opts.FirstBlock = 0
	opts.LastBlock = utils.NOPOS
	for key, value := range r.URL.Query() {
		switch key {
		case "addrs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs = append(opts.Addrs, s...)
			}
		case "count":
			opts.Count = true
		case "noZero":
			opts.NoZero = true
		case "bounds":
			opts.Bounds = true
		case "unripe":
			opts.Unripe = true
		case "silent":
			opts.Silent = true
		case "firstRecord":
			opts.FirstRecord = globals.ToUint64(value[0])
		case "maxRecords":
			opts.MaxRecords = globals.ToUint64(value[0])
		case "reversed":
			opts.Reversed = true
		case "publisher":
			opts.Publisher = value[0]
		case "firstBlock":
			opts.FirstBlock = globals.ToUint64(value[0])
		case "lastBlock":
			opts.LastBlock = globals.ToUint64(value[0])
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "list")
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, r, opts.getCaches())
	opts.Publisher, _ = opts.Conn.GetEnsAddress(opts.Publisher)
	opts.PublisherAddr = base.HexToAddress(opts.Publisher)

	// EXISTING_CODE
	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(opts.Addrs)

	return opts
}

// listFinishParse finishes the parsing for command line invocations. Returns a new ListOptions.
func listFinishParse(args []string) *ListOptions {
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
	opts.Publisher, _ = opts.Conn.GetEnsAddress(opts.Publisher)
	opts.PublisherAddr = base.HexToAddress(opts.Publisher)

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

func GetOptions() *ListOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultListOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultListOptions = ListOptions{}
	globals.SetDefaults(&defaultListOptions.Globals)
	defaultListOptions.Globals.Writer = w
	capabilities := caps.Default // Additional global caps for chifra list
	// EXISTING_CODE
	// EXISTING_CODE
	defaultListOptions.Globals.Caps = capabilities
}

func (opts *ListOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
