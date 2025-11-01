// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package abisPkg

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

// AbisOptions provides all command options for the chifra abis command.
type AbisOptions struct {
	Addrs    []string              `json:"addrs,omitempty"`    // A list of one or more smart contracts whose ABIs to display
	Known    bool                  `json:"known,omitempty"`    // Load common 'known' ABIs from cache
	ProxyFor string                `json:"proxyFor,omitempty"` // Redirects the query to this implementation
	List     bool                  `json:"list,omitempty"`     // A list of downloaded abi files
	Details  bool                  `json:"details,omitempty"`  // Show the functions and events instead of summaries for all abi files
	Count    bool                  `json:"count,omitempty"`    // Show the number of abis downloaded
	Find     []string              `json:"find,omitempty"`     // Search for function or event declarations given a four- or 32-byte code(s)
	Hint     []string              `json:"hint,omitempty"`     // For the --find option only, provide hints to speed up the search
	Encode   string                `json:"encode,omitempty"`   // Generate the 32-byte encoding for a given canonical function or event signature
	Globals  globals.GlobalOptions `json:"globals,omitempty"`  // The global options
	Conn     *rpc.Connection       `json:"conn,omitempty"`     // The connection to the RPC server
	BadFlag  error                 `json:"badFlag,omitempty"`  // An error flag if needed
	// EXISTING_CODE
	ProxyForAddr base.Address `json:"-"`
	// EXISTING_CODE
}

var defaultAbisOptions = AbisOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *AbisOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(opts.Known, "Known: ", opts.Known)
	logger.TestLog(len(opts.ProxyFor) > 0, "ProxyFor: ", opts.ProxyFor)
	logger.TestLog(opts.List, "List: ", opts.List)
	logger.TestLog(opts.Details, "Details: ", opts.Details)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(len(opts.Find) > 0, "Find: ", opts.Find)
	logger.TestLog(len(opts.Hint) > 0, "Hint: ", opts.Hint)
	logger.TestLog(len(opts.Encode) > 0, "Encode: ", opts.Encode)
	opts.Conn.TestLog()
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *AbisOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// abisFinishParseApi finishes the parsing for server invocations. Returns a new AbisOptions.
func abisFinishParseApi(w http.ResponseWriter, r *http.Request) *AbisOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return AbisFinishParseInternal(w, values)
}

func AbisFinishParseInternal(w io.Writer, values url.Values) *AbisOptions {
	copy := defaultAbisOptions
	copy.Globals.Caps = getCaps()
	opts := &copy
	for key, value := range values {
		switch key {
		case "addrs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs = append(opts.Addrs, s...)
			}
		case "known":
			opts.Known = true
		case "proxyFor":
			opts.ProxyFor = value[0]
		case "list":
			opts.List = true
		case "details":
			opts.Details = true
		case "count":
			opts.Count = true
		case "find":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Find = append(opts.Find, s...)
			}
		case "hint":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Hint = append(opts.Hint, s...)
			}
		case "encode":
			opts.Encode = value[0]
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "abis")
				if opts.BadFlag == nil || opts.BadFlag.Error() > err.Error() {
					opts.BadFlag = err
				}
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())
	opts.ProxyFor, _ = opts.Conn.GetEnsAddress(opts.ProxyFor)
	opts.ProxyForAddr = base.HexToAddress(opts.ProxyFor)

	// EXISTING_CODE
	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(opts.Addrs)

	return opts
}

// abisFinishParse finishes the parsing for command line invocations. Returns a new AbisOptions.
func abisFinishParse(args []string) *AbisOptions {
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
	opts.ProxyFor, _ = opts.Conn.GetEnsAddress(opts.ProxyFor)
	opts.ProxyForAddr = base.HexToAddress(opts.ProxyFor)

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

func GetOptions() *AbisOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultAbisOptions
}

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra abis
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Names)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	opts := AbisOptions{}
	globals.SetDefaults(&opts.Globals)
	opts.Globals.TestMode = testMode
	opts.Globals.Writer = w
	opts.Globals.Caps = getCaps()
	// EXISTING_CODE
	// EXISTING_CODE
	defaultAbisOptions = opts
}

func (opts *AbisOptions) getCaches() (caches map[walk.CacheType]bool) {
	// EXISTING_CODE
	caches = map[walk.CacheType]bool{
		walk.Cache_Abis: true,
	}
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
