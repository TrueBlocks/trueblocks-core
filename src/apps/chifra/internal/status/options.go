// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package statusPkg

import (
	"encoding/json"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// StatusOptions provides all command options for the chifra status command.
type StatusOptions struct {
	Modes       []string              `json:"modes,omitempty"`       // The (optional) name of the binary cache to report on, terse otherwise
	FirstRecord uint64                `json:"firstRecord,omitempty"` // The first record to process
	MaxRecords  uint64                `json:"maxRecords,omitempty"`  // The maximum number of records to process
	Globals     globals.GlobalOptions `json:"globals,omitempty"`     // The global options
	Conn        *rpcClient.Connection `json:"conn,omitempty"`        // The connection to the RPC server
	BadFlag     error                 `json:"badFlag,omitempty"`     // An error flag if needed
	// EXISTING_CODE
	ModeTypes []walk.CacheType `json:"-"`
	// EXISTING_CODE
}

var defaultStatusOptions = StatusOptions{
	MaxRecords: 10000,
}

// testLog is used only during testing to export the options for this test case.
func (opts *StatusOptions) testLog() {
	logger.TestLog(len(opts.Modes) > 0, "Modes: ", opts.Modes)
	logger.TestLog(opts.FirstRecord != 0, "FirstRecord: ", opts.FirstRecord)
	logger.TestLog(opts.MaxRecords != 10000, "MaxRecords: ", opts.MaxRecords)
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *StatusOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// statusFinishParseApi finishes the parsing for server invocations. Returns a new StatusOptions.
func statusFinishParseApi(w http.ResponseWriter, r *http.Request) *StatusOptions {
	copy := defaultStatusOptions
	opts := &copy
	opts.FirstRecord = 0
	opts.MaxRecords = 10000
	for key, value := range r.URL.Query() {
		switch key {
		case "modes":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Modes = append(opts.Modes, s...)
			}
		case "firstRecord":
			opts.FirstRecord = globals.ToUint64(value[0])
		case "maxRecords":
			opts.MaxRecords = globals.ToUint64(value[0])
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "status")
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, r, opts.getCaches())

	// EXISTING_CODE
	if len(opts.Modes) == 0 && opts.Globals.Verbose {
		opts.Modes = append(opts.Modes, "some")
	}
	opts.ModeTypes = getCacheTypes(opts.Modes)
	// EXISTING_CODE

	return opts
}

// statusFinishParse finishes the parsing for command line invocations. Returns a new StatusOptions.
func statusFinishParse(args []string) *StatusOptions {
	defFmt := "txt"
	opts := GetOptions()
	opts.Conn = opts.Globals.FinishParse(args, opts.getCaches())

	// EXISTING_CODE
	opts.Modes = append(opts.Modes, args...)
	if len(opts.Modes) == 0 && opts.Globals.Verbose {
		opts.Modes = append(opts.Modes, "some")
	}
	opts.ModeTypes = getCacheTypes(opts.Modes)
	if len(opts.Modes) > 0 {
		defFmt = "json"
	}
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *StatusOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultStatusOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultStatusOptions = StatusOptions{}
	globals.SetDefaults(&defaultStatusOptions.Globals)
	defaultStatusOptions.Globals.Writer = w
	capabilities := caps.Default // Additional global caps for chifra status
	// EXISTING_CODE
	// EXISTING_CODE
	defaultStatusOptions.Globals.Caps = capabilities
}

func (opts *StatusOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
func getCacheTypes(strs []string) []walk.CacheType {
	haveit := map[string]bool{} // removes dups
	var types []walk.CacheType
	for _, str := range strs {
		if !haveit[str] {
			haveit[str] = true
			switch str {
			case "abis":
				types = append(types, walk.Cache_Abis)
			case "blocks":
				types = append(types, walk.Cache_Blocks)
			case "monitors":
				types = append(types, walk.Cache_Monitors)
			case "names":
				types = append(types, walk.Cache_Names)
			case "recons":
				types = append(types, walk.Cache_Recons)
			case "slurps":
				types = append(types, walk.Cache_Slurps)
			case "tmp":
				types = append(types, walk.Cache_Tmp)
			case "traces":
				types = append(types, walk.Cache_Traces)
			case "txs":
				types = append(types, walk.Cache_Transactions)
			case "blooms":
				types = append(types, walk.Index_Bloom)
			case "index":
				fallthrough
			case "finalized":
				types = append(types, walk.Index_Final)
			case "ripe":
				types = append(types, walk.Index_Ripe)
			case "staging":
				types = append(types, walk.Index_Staging)
			case "unripe":
				types = append(types, walk.Index_Unripe)
			case "maps":
				types = append(types, walk.Index_Maps)
			case "some":
				types = append(types, walk.Index_Final)
				types = append(types, walk.Cache_Monitors)
				types = append(types, walk.Cache_Names)
				types = append(types, walk.Cache_Abis)
				types = append(types, walk.Cache_Slurps)
			case "all":
				types = append(types, walk.Index_Bloom)
				types = append(types, walk.Index_Final)
				types = append(types, walk.Index_Staging)
				types = append(types, walk.Index_Unripe)
				types = append(types, walk.Cache_Monitors)
				types = append(types, walk.Cache_Names)
				types = append(types, walk.Cache_Abis)
				types = append(types, walk.Cache_Slurps)
				types = append(types, walk.Cache_Blocks)
				types = append(types, walk.Cache_Traces)
				types = append(types, walk.Cache_Transactions)
			}
		}
	}
	/*
		all:     abis|monitors|names|slurps|blocks|traces|txs|recons|tmp|blooms|index|finalized|staging|ripe|unripe|maps
		cmd:     abis|monitors|names|slurps|blocks|traces|txs|index|some|all
		missing: recons|tmp|blooms|finalized|staging|ripe|unripe|maps
	*/
	return types
}

// EXISTING_CODE
