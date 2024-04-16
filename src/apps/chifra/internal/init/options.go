// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package initPkg

import (
	// EXISTING_CODE
	"encoding/json"
	"io"
	"net/http"
	"net/url"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	// EXISTING_CODE
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
	PublisherAddr base.Address `json:"-"`
	// EXISTING_CODE
}

var defaultInitOptions = InitOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *InitOptions) testLog() {
	logger.TestLog(opts.All, "All: ", opts.All)
	logger.TestLog(opts.DryRun, "DryRun: ", opts.DryRun)
	logger.TestLog(len(opts.Publisher) > 0, "Publisher: ", opts.Publisher)
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
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return InitFinishParseInternal(w, values)
}

func InitFinishParseInternal(w io.Writer, values url.Values) *InitOptions {
	copy := defaultInitOptions
	opts := &copy
	for key, value := range values {
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
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "init")
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
	opts.Publisher, _ = opts.Conn.GetEnsAddress(config.GetPublisher(opts.Publisher))
	opts.PublisherAddr = base.HexToAddress(opts.Publisher)

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

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultInitOptions = InitOptions{}
	globals.SetDefaults(&defaultInitOptions.Globals)
	defaultInitOptions.Globals.TestMode = testMode
	defaultInitOptions.Globals.Writer = w
	var capabilities caps.Capability // capabilities for chifra init
	capabilities = capabilities.Add(caps.Verbose)
	capabilities = capabilities.Add(caps.Version)
	capabilities = capabilities.Add(caps.Noop)
	capabilities = capabilities.Add(caps.NoColor)
	capabilities = capabilities.Add(caps.Chain)
	// EXISTING_CODE
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
