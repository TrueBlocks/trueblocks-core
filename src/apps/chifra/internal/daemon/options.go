// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package daemonPkg

import (
	// EXISTING_CODE
	"encoding/json"
	"io"
	"net/http"
	"net/url"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
	// EXISTING_CODE
)

// DaemonOptions provides all command options for the chifra daemon command.
type DaemonOptions struct {
	Url     string                `json:"url,omitempty"`     // Specify the API server's url and optionally its port
	Silent  bool                  `json:"silent,omitempty"`  // Disable logging (for use in SDK for example)
	Globals globals.GlobalOptions `json:"globals,omitempty"` // The global options
	Conn    *rpc.Connection       `json:"conn,omitempty"`    // The connection to the RPC server
	BadFlag error                 `json:"badFlag,omitempty"` // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultDaemonOptions = DaemonOptions{
	Url: "localhost:8080",
}

// testLog is used only during testing to export the options for this test case.
func (opts *DaemonOptions) testLog() {
	logger.TestLog(len(opts.Url) > 0 && opts.Url != "localhost:8080", "Url: ", opts.Url)
	logger.TestLog(opts.Silent, "Silent: ", opts.Silent)
	opts.Conn.TestLog()
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *DaemonOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// daemonFinishParseApi finishes the parsing for server invocations. Returns a new DaemonOptions.
func daemonFinishParseApi(w http.ResponseWriter, r *http.Request) *DaemonOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return DaemonFinishParseInternal(w, values)
}

func DaemonFinishParseInternal(w io.Writer, values url.Values) *DaemonOptions {
	copy := defaultDaemonOptions
	copy.Globals.Caps = getCaps()
	opts := &copy
	opts.Url = "localhost:8080"
	for key, value := range values {
		switch key {
		case "url":
			opts.Url = value[0]
		case "silent":
			opts.Silent = true
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "daemon")
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

// daemonFinishParse finishes the parsing for command line invocations. Returns a new DaemonOptions.
func daemonFinishParse(args []string) *DaemonOptions {
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
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *DaemonOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultDaemonOptions
}

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra daemon
	capabilities = capabilities.Add(caps.Verbose)
	capabilities = capabilities.Add(caps.Version)
	capabilities = capabilities.Add(caps.Noop)
	capabilities = capabilities.Add(caps.NoColor)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	opts := DaemonOptions{}
	globals.SetDefaults(&opts.Globals)
	opts.Globals.TestMode = testMode
	opts.Globals.Writer = w
	opts.Globals.Caps = getCaps()
	opts.Url = "localhost:8080"
	defaultDaemonOptions = opts
}

func (opts *DaemonOptions) getCaches() (caches map[walk.CacheType]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
