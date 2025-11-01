// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package statusPkg

import (
	// EXISTING_CODE
	"encoding/json"
	"io"
	"net/http"
	"net/url"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/walk"
	// EXISTING_CODE
)

// StatusOptions provides all command options for the chifra status command.
type StatusOptions struct {
	Modes       []string              `json:"modes,omitempty"`       // The (optional) name of the binary cache to report on, terse otherwise
	Diagnose    bool                  `json:"diagnose,omitempty"`    // Same as the default but with additional diagnostics
	FirstRecord uint64                `json:"firstRecord,omitempty"` // The first record to process
	MaxRecords  uint64                `json:"maxRecords,omitempty"`  // The maximum number of records to process
	Chains      bool                  `json:"chains,omitempty"`      // Include a list of chain configurations in the output
	Caches      bool                  `json:"caches,omitempty"`      // Include a list of cache items in the output
	Healthcheck bool                  `json:"healthcheck,omitempty"` // An alias for the diagnose endpoint
	Globals     globals.GlobalOptions `json:"globals,omitempty"`     // The global options
	Conn        *rpc.Connection       `json:"conn,omitempty"`        // The connection to the RPC server
	BadFlag     error                 `json:"badFlag,omitempty"`     // An error flag if needed
	// EXISTING_CODE
	ModeTypes []walk.CacheType `json:"-"`
	OrigModes []string         `json:"-"`
	// EXISTING_CODE
}

var defaultStatusOptions = StatusOptions{
	MaxRecords: 10000,
}

// testLog is used only during testing to export the options for this test case.
func (opts *StatusOptions) testLog() {
	logger.TestLog(len(opts.Modes) > 0, "Modes: ", opts.Modes)
	logger.TestLog(opts.Diagnose, "Diagnose: ", opts.Diagnose)
	logger.TestLog(opts.FirstRecord != 0, "FirstRecord: ", opts.FirstRecord)
	logger.TestLog(opts.MaxRecords != 10000, "MaxRecords: ", opts.MaxRecords)
	logger.TestLog(opts.Chains, "Chains: ", opts.Chains)
	logger.TestLog(opts.Caches, "Caches: ", opts.Caches)
	logger.TestLog(opts.Healthcheck, "Healthcheck: ", opts.Healthcheck)
	opts.Conn.TestLog()
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *StatusOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// statusFinishParseApi finishes the parsing for server invocations. Returns a new StatusOptions.
func statusFinishParseApi(w http.ResponseWriter, r *http.Request) *StatusOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return StatusFinishParseInternal(w, values)
}

func StatusFinishParseInternal(w io.Writer, values url.Values) *StatusOptions {
	copy := defaultStatusOptions
	copy.Globals.Caps = getCaps()
	opts := &copy
	opts.MaxRecords = 10000
	for key, value := range values {
		switch key {
		case "modes":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Modes = append(opts.Modes, s...)
			}
		case "diagnose":
			opts.Diagnose = true
		case "firstRecord":
			opts.FirstRecord = base.MustParseUint64(value[0])
		case "maxRecords":
			opts.MaxRecords = base.MustParseUint64(value[0])
		case "chains":
			opts.Chains = true
		case "caches":
			opts.Caches = true
		case "healthcheck":
			opts.Healthcheck = true
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "status")
				if opts.BadFlag == nil || opts.BadFlag.Error() > err.Error() {
					opts.BadFlag = err
				}
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())

	// EXISTING_CODE
	opts.OrigModes = opts.Modes
	if len(opts.Modes) == 0 {
		if opts.Caches || opts.Chains {
			opts.Modes = append(opts.Modes, "all")
		} else if opts.Globals.Verbose {
			opts.Modes = append(opts.Modes, "some")
		}
	}
	opts.ModeTypes = walk.CacheTypesFromStringSlice(opts.Modes)
	// EXISTING_CODE
	if !opts.Diagnose {
		opts.Diagnose = opts.Healthcheck // alias
		opts.Healthcheck = false
	}

	return opts
}

// statusFinishParse finishes the parsing for command line invocations. Returns a new StatusOptions.
func statusFinishParse(args []string) *StatusOptions {
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
	opts.Modes = append(opts.Modes, args...)
	opts.OrigModes = opts.Modes
	if len(opts.Modes) == 0 {
		if opts.Caches || opts.Chains {
			opts.Modes = append(opts.Modes, "all")
		} else if opts.Globals.Verbose {
			opts.Modes = append(opts.Modes, "some")
		}
	}
	opts.ModeTypes = walk.CacheTypesFromStringSlice(opts.Modes)
	if len(opts.Modes) > 0 {
		defFmt = "json"
	}
	// EXISTING_CODE
	if !opts.Diagnose {
		opts.Diagnose = opts.Healthcheck // alias
		opts.Healthcheck = false
	}
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

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra status
	capabilities = capabilities.Add(caps.Default)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	opts := StatusOptions{}
	globals.SetDefaults(&opts.Globals)
	opts.Globals.TestMode = testMode
	opts.Globals.Writer = w
	opts.Globals.Caps = getCaps()
	// EXISTING_CODE
	// EXISTING_CODE
	opts.MaxRecords = 10000
	defaultStatusOptions = opts
}

func (opts *StatusOptions) getCaches() (caches map[walk.CacheType]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
