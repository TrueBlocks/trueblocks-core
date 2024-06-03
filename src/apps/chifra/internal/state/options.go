// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package statePkg

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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
	// EXISTING_CODE
)

// StateOptions provides all command options for the chifra state command.
type StateOptions struct {
	Addrs      []string                 `json:"addrs,omitempty"`      // One or more addresses (0x...) from which to retrieve balances
	Blocks     []string                 `json:"blocks,omitempty"`     // An optional list of one or more blocks at which to report balances, defaults to 'latest'
	BlockIds   []identifiers.Identifier `json:"blockIds,omitempty"`   // Block identifiers
	Parts      []string                 `json:"parts,omitempty"`      // Control which state to export
	Changes    bool                     `json:"changes,omitempty"`    // Only report a balance when it changes from one block to the next
	NoZero     bool                     `json:"noZero,omitempty"`     // Suppress the display of zero balance accounts
	Call       string                   `json:"call,omitempty"`       // Call a smart contract with one or more solidity calls, four-byte plus parameters, or encoded call data strings
	Articulate bool                     `json:"articulate,omitempty"` // For the --call option only, articulate the retrieved data if ABIs can be found
	ProxyFor   string                   `json:"proxyFor,omitempty"`   // For the --call option only, redirects calls to this implementation
	Globals    globals.GlobalOptions    `json:"globals,omitempty"`    // The global options
	Conn       *rpc.Connection          `json:"conn,omitempty"`       // The connection to the RPC server
	BadFlag    error                    `json:"badFlag,omitempty"`    // An error flag if needed
	// EXISTING_CODE
	Calls []string `json:"-"`
	// EXISTING_CODE
}

var defaultStateOptions = StateOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *StateOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Parts) > 0, "Parts: ", opts.Parts)
	logger.TestLog(opts.Changes, "Changes: ", opts.Changes)
	logger.TestLog(opts.NoZero, "NoZero: ", opts.NoZero)
	logger.TestLog(len(opts.Call) > 0, "Call: ", opts.Call)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	logger.TestLog(len(opts.ProxyFor) > 0, "ProxyFor: ", opts.ProxyFor)
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *StateOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// stateFinishParseApi finishes the parsing for server invocations. Returns a new StateOptions.
func stateFinishParseApi(w http.ResponseWriter, r *http.Request) *StateOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return StateFinishParseInternal(w, values)
}

func StateFinishParseInternal(w io.Writer, values url.Values) *StateOptions {
	copy := defaultStateOptions
	copy.Globals.Caps = getCaps()
	opts := &copy
	for key, value := range values {
		switch key {
		case "addrs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs = append(opts.Addrs, s...)
			}
		case "blocks":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Blocks = append(opts.Blocks, s...)
			}
		case "parts":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Parts = append(opts.Parts, s...)
			}
		case "changes":
			opts.Changes = true
		case "noZero":
			opts.NoZero = true
		case "call":
			opts.Call = value[0]
		case "articulate":
			opts.Articulate = true
		case "proxyFor":
			opts.ProxyFor = value[0]
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "state")
				if opts.BadFlag == nil || opts.BadFlag.Error() > err.Error() {
					opts.BadFlag = err
				}
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())

	// EXISTING_CODE
	opts.Call = strings.Replace(strings.Trim(opts.Call, "'"), "'", "\"", -1)
	opts.Calls = strings.Split(opts.Call, ":")
	if len(opts.Blocks) == 0 {
		if opts.Globals.TestMode {
			opts.Blocks = []string{"17000000"}
		} else {
			opts.Blocks = []string{"latest"}
		}
	}
	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(opts.Addrs)
	opts.ProxyFor, _ = opts.Conn.GetEnsAddress(opts.ProxyFor)

	return opts
}

// stateFinishParse finishes the parsing for command line invocations. Returns a new StateOptions.
func stateFinishParse(args []string) *StateOptions {
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
		} else {
			opts.Blocks = append(opts.Blocks, arg)
		}
	}
	opts.Call = strings.Replace(strings.Trim(opts.Call, "'"), "'", "\"", -1)
	opts.Calls = strings.Split(opts.Call, ":")
	if len(opts.Blocks) == 0 {
		if opts.Globals.TestMode {
			opts.Blocks = []string{"17000000"}
		} else {
			opts.Blocks = []string{"latest"}
		}
	}
	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(opts.Addrs)
	opts.ProxyFor, _ = opts.Conn.GetEnsAddress(opts.ProxyFor)
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *StateOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultStateOptions
}

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra state
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Ether)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	opts := StateOptions{}
	globals.SetDefaults(&opts.Globals)
	opts.Globals.TestMode = testMode
	opts.Globals.Writer = w
	opts.Globals.Caps = getCaps()
	defaultStateOptions = opts
}

func (opts *StateOptions) getCaches() (caches map[walk.CacheType]bool) {
	// EXISTING_CODE
	caches = map[walk.CacheType]bool{
		walk.Cache_State:   true,
		walk.Cache_Results: len(opts.Call) > 0,
	}
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
