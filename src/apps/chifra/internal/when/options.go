// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package whenPkg

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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/walk"
	// EXISTING_CODE
)

// WhenOptions provides all command options for the chifra when command.
type WhenOptions struct {
	Blocks     []string                 `json:"blocks,omitempty"`     // One or more dates, block numbers, hashes, or special named blocks (see notes)
	BlockIds   []identifiers.Identifier `json:"blockIds,omitempty"`   // Block identifiers
	List       bool                     `json:"list,omitempty"`       // Export a list of the 'special' blocks
	Timestamps bool                     `json:"timestamps,omitempty"` // Display or process timestamps
	Count      bool                     `json:"count,omitempty"`      // With --timestamps only, returns the number of timestamps in the cache
	Truncate   base.Blknum              `json:"truncate,omitempty"`   // With --timestamps only, truncates the timestamp file at this block
	Repair     bool                     `json:"repair,omitempty"`     // With --timestamps only, repairs block(s) in the block range by re-querying from the chain
	Check      bool                     `json:"check,omitempty"`      // With --timestamps only, checks the validity of the timestamp data
	Update     bool                     `json:"update,omitempty"`     // With --timestamps only, bring the timestamp database forward to the latest block
	Deep       uint64                   `json:"deep,omitempty"`       // With --timestamps --check only, verifies every N timestamp directly from the chain (slow)
	Globals    globals.GlobalOptions    `json:"globals,omitempty"`    // The global options
	Conn       *rpc.Connection          `json:"conn,omitempty"`       // The connection to the RPC server
	BadFlag    error                    `json:"badFlag,omitempty"`    // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultWhenOptions = WhenOptions{
	Truncate: base.NOPOSN,
}

// testLog is used only during testing to export the options for this test case.
func (opts *WhenOptions) testLog() {
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(opts.List, "List: ", opts.List)
	logger.TestLog(opts.Timestamps, "Timestamps: ", opts.Timestamps)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(opts.Truncate != base.NOPOSN, "Truncate: ", opts.Truncate)
	logger.TestLog(opts.Repair, "Repair: ", opts.Repair)
	logger.TestLog(opts.Check, "Check: ", opts.Check)
	logger.TestLog(opts.Update, "Update: ", opts.Update)
	logger.TestLog(opts.Deep != 0, "Deep: ", opts.Deep)
	opts.Conn.TestLog()
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *WhenOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// whenFinishParseApi finishes the parsing for server invocations. Returns a new WhenOptions.
func whenFinishParseApi(w http.ResponseWriter, r *http.Request) *WhenOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return WhenFinishParseInternal(w, values)
}

func WhenFinishParseInternal(w io.Writer, values url.Values) *WhenOptions {
	copy := defaultWhenOptions
	copy.Globals.Caps = getCaps()
	opts := &copy
	opts.Truncate = base.NOPOSN
	for key, value := range values {
		switch key {
		case "blocks":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Blocks = append(opts.Blocks, s...)
			}
		case "list":
			opts.List = true
		case "timestamps":
			opts.Timestamps = true
		case "count":
			opts.Count = true
		case "truncate":
			opts.Truncate = base.MustParseBlknum(value[0])
		case "repair":
			opts.Repair = true
		case "check":
			opts.Check = true
		case "update":
			opts.Update = true
		case "deep":
			opts.Deep = base.MustParseUint64(value[0])
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "when")
				if opts.BadFlag == nil || opts.BadFlag.Error() > err.Error() {
					opts.BadFlag = err
				}
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())

	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// whenFinishParse finishes the parsing for command line invocations. Returns a new WhenOptions.
func whenFinishParse(args []string) *WhenOptions {
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

	// EXISTING_CODE
	opts.Blocks = args
	if opts.Truncate == 0 {
		opts.Truncate = base.NOPOSN
	}
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *WhenOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultWhenOptions
}

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra when
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	opts := WhenOptions{}
	globals.SetDefaults(&opts.Globals)
	opts.Globals.TestMode = testMode
	opts.Globals.Writer = w
	opts.Globals.Caps = getCaps()
	// EXISTING_CODE
	// EXISTING_CODE
	opts.Truncate = base.NOPOSN
	defaultWhenOptions = opts
}

func (opts *WhenOptions) getCaches() (caches map[walk.CacheType]bool) {
	// EXISTING_CODE
	caches = map[walk.CacheType]bool{
		walk.Cache_Blocks: !opts.Timestamps,
	}
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
