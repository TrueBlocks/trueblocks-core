// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package initPkg

import (
	"encoding/json"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// InitOptions provides all command options for the chifra init command.
type InitOptions struct {
	All        bool                  `json:"all,omitempty"`        // In addition to Bloom filters, download full index chunks (recommended)
	DryRun     bool                  `json:"dryRun,omitempty"`     // Display the results of the download without actually downloading
	Publisher  string                `json:"publisher,omitempty"`  // The publisher of the index to download
	FirstBlock uint64                `json:"firstBlock,omitempty"` // Do not download any chunks earlier than this block
	Sleep      float64               `json:"sleep,omitempty"`      // Seconds to sleep between downloads
	Globals    globals.GlobalOptions `json:"globals,omitempty"`    // The global options
	Conn       *rpc.Connection       `json:"conn,omitempty"`       // The connection to the RPC server
	BadFlag    error                 `json:"badFlag,omitempty"`    // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultInitOptions = InitOptions{
	Publisher: "trueblocks.eth",
}

// testLog is used only during testing to export the options for this test case.
func (opts *InitOptions) testLog() {
	logger.TestLog(opts.All, "All: ", opts.All)
	logger.TestLog(opts.DryRun, "DryRun: ", opts.DryRun)
	logger.TestLog(len(opts.Publisher) > 0 && opts.Publisher != "trueblocks.eth", "Publisher: ", opts.Publisher)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.Sleep != float64(0.0), "Sleep: ", opts.Sleep)
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *InitOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// initFinishParseApi finishes the parsing for server invocations. Returns a new InitOptions.
func initFinishParseApi(w http.ResponseWriter, r *http.Request) *InitOptions {
	copy := defaultInitOptions
	opts := &copy
	opts.FirstBlock = 0
	opts.Sleep = 0.0
	for key, value := range r.URL.Query() {
		switch key {
		case "all":
			opts.All = true
		case "dryRun":
			opts.DryRun = true
		case "publisher":
			opts.Publisher = value[0]
		case "firstBlock":
			opts.FirstBlock = globals.ToUint64(value[0])
		case "sleep":
			opts.Sleep = globals.ToFloat64(value[0])
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "init")
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, r, opts.getCaches())
	opts.Publisher, _ = opts.Conn.GetEnsAddress(opts.Publisher)

	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

// initFinishParse finishes the parsing for command line invocations. Returns a new InitOptions.
func initFinishParse(args []string) *InitOptions {
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

	// EXISTING_CODE
	if len(args) > 0 {
		opts.BadFlag = validate.Usage("Invalid argument ({0}).", args[0])
	}
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *InitOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultInitOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultInitOptions = InitOptions{}
	globals.SetDefaults(&defaultInitOptions.Globals)
	defaultInitOptions.Globals.Writer = w
	capabilities := caps.Default // Additional global caps for chifra init
	// EXISTING_CODE
	capabilities = capabilities.Remove(caps.Fmt)
	capabilities = capabilities.Remove(caps.NoHeader)
	capabilities = capabilities.Remove(caps.File)
	capabilities = capabilities.Remove(caps.Output)
	capabilities = capabilities.Remove(caps.Append)
	// EXISTING_CODE
	defaultInitOptions.Globals.Caps = capabilities
}

func (opts *InitOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
