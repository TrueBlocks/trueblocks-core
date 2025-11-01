// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package listPkg

import (
	// EXISTING_CODE
	"encoding/json"
	"io"
	"net/http"
	"net/url"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/walk"
	// EXISTING_CODE
)

// ListOptions provides all command options for the chifra list command.
type ListOptions struct {
	Addrs       []string              `json:"addrs,omitempty"`       // One or more addresses (0x...) to list
	Count       bool                  `json:"count,omitempty"`       // Display only the count of records for each monitor
	NoZero      bool                  `json:"noZero,omitempty"`      // For the --count option only, suppress the display of zero appearance accounts
	Bounds      bool                  `json:"bounds,omitempty"`      // Report first and last block this address appears
	Unripe      bool                  `json:"unripe,omitempty"`      // List transactions labeled unripe (i.e. less than 28 blocks old)
	Silent      bool                  `json:"silent,omitempty"`      // Freshen the monitor only (no reporting)
	FirstRecord uint64                `json:"firstRecord,omitempty"` // The first record to process
	MaxRecords  uint64                `json:"maxRecords,omitempty"`  // The maximum number of records to process
	Reversed    bool                  `json:"reversed,omitempty"`    // Produce results in reverse chronological order
	Publisher   string                `json:"publisher,omitempty"`   // For some query options, the publisher of the index
	FirstBlock  base.Blknum           `json:"firstBlock,omitempty"`  // First block to export (inclusive, ignored when freshening)
	LastBlock   base.Blknum           `json:"lastBlock,omitempty"`   // Last block to export (inclusive, ignored when freshening)
	Globals     globals.GlobalOptions `json:"globals,omitempty"`     // The global options
	Conn        *rpc.Connection       `json:"conn,omitempty"`        // The connection to the RPC server
	BadFlag     error                 `json:"badFlag,omitempty"`     // An error flag if needed
	// EXISTING_CODE
	PublisherAddr base.Address `json:"-"`
	// EXISTING_CODE
}

var defaultListOptions = ListOptions{
	MaxRecords: 250,
	LastBlock:  base.NOPOSN,
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
	logger.TestLog(len(opts.Publisher) > 0, "Publisher: ", opts.Publisher)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.LastBlock != base.NOPOSN && opts.LastBlock != 0, "LastBlock: ", opts.LastBlock)
	opts.Conn.TestLog()
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ListOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// listFinishParseApi finishes the parsing for server invocations. Returns a new ListOptions.
func listFinishParseApi(w http.ResponseWriter, r *http.Request) *ListOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return ListFinishParseInternal(w, values)
}

func ListFinishParseInternal(w io.Writer, values url.Values) *ListOptions {
	copy := defaultListOptions
	copy.Globals.Caps = getCaps()
	opts := &copy
	opts.MaxRecords = 250
	opts.LastBlock = base.NOPOSN
	for key, value := range values {
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
			opts.FirstRecord = base.MustParseUint64(value[0])
		case "maxRecords":
			opts.MaxRecords = base.MustParseUint64(value[0])
		case "reversed":
			opts.Reversed = true
		case "publisher":
			opts.Publisher = value[0]
		case "firstBlock":
			opts.FirstBlock = base.MustParseBlknum(value[0])
		case "lastBlock":
			opts.LastBlock = base.MustParseBlknum(value[0])
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "list")
				if opts.BadFlag == nil || opts.BadFlag.Error() > err.Error() {
					opts.BadFlag = err
				}
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())
	opts.Publisher, _ = opts.Conn.GetEnsAddress(config.GetPublisher(opts.Publisher))
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
			if cnt := dedup[arg]; cnt == 0 {
				tmp = append(tmp, arg)
			}
			dedup[arg]++
		}
		args = tmp
	}

	defFmt := "txt"
	opts := GetOptions()
	opts.Conn = opts.Globals.FinishParse(args, opts.getCaches())
	opts.Publisher, _ = opts.Conn.GetEnsAddress(config.GetPublisher(opts.Publisher))
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

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra list
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Names)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	opts := ListOptions{}
	globals.SetDefaults(&opts.Globals)
	opts.Globals.TestMode = testMode
	opts.Globals.Writer = w
	opts.Globals.Caps = getCaps()
	// EXISTING_CODE
	// EXISTING_CODE
	opts.MaxRecords = 250
	opts.LastBlock = base.NOPOSN
	defaultListOptions = opts
}

func (opts *ListOptions) getCaches() (caches map[walk.CacheType]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
