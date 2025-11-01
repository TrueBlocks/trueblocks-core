// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package namesPkg

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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/crud"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/walk"
	// EXISTING_CODE
)

// NamesOptions provides all command options for the chifra names command.
type NamesOptions struct {
	Terms     []string              `json:"terms,omitempty"`     // A space separated list of one or more search terms
	Expand    bool                  `json:"expand,omitempty"`    // Expand search to include all fields (search name, address, and symbol otherwise)
	MatchCase bool                  `json:"matchCase,omitempty"` // Do case-sensitive search
	All       bool                  `json:"all,omitempty"`       // Include all (including custom) names in the search
	Custom    bool                  `json:"custom,omitempty"`    // Include only custom named accounts in the search
	Prefund   bool                  `json:"prefund,omitempty"`   // Include prefund accounts in the search
	Addr      bool                  `json:"addr,omitempty"`      // Display only addresses in the results (useful for scripting, assumes --no_header)
	Tags      bool                  `json:"tags,omitempty"`      // Export the list of tags and subtags only
	Clean     bool                  `json:"clean,omitempty"`     // Clean the data (addrs to lower case, sort by addr)
	Regular   bool                  `json:"regular,omitempty"`   // Only available with --clean, cleans regular names database
	Count     bool                  `json:"count,omitempty"`     // Return the number of names matching the search terms or other options
	DryRun    bool                  `json:"dryRun,omitempty"`    // Only available with --clean or --autoname, outputs changes to stdout instead of updating databases
	Autoname  string                `json:"autoname,omitempty"`  // An address assumed to be a token, added automatically to names database if true
	Create    bool                  `json:"create,omitempty"`    // Create a new name record
	Update    bool                  `json:"update,omitempty"`    // Edit an existing name
	Delete    bool                  `json:"delete,omitempty"`    // Delete a name, but do not remove it
	Undelete  bool                  `json:"undelete,omitempty"`  // Undelete a previously deleted name
	Remove    bool                  `json:"remove,omitempty"`    // Remove a previously deleted name
	Globals   globals.GlobalOptions `json:"globals,omitempty"`   // The global options
	Conn      *rpc.Connection       `json:"conn,omitempty"`      // The connection to the RPC server
	BadFlag   error                 `json:"badFlag,omitempty"`   // An error flag if needed
	// EXISTING_CODE
	crudData     *crud.NameCrud
	AutonameAddr base.Address `json:"-"`
	OrigTerms    []string     `json:"-"`
	// EXISTING_CODE
}

var defaultNamesOptions = NamesOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *NamesOptions) testLog() {
	logger.TestLog(len(opts.Terms) > 0, "Terms: ", opts.Terms)
	logger.TestLog(opts.Expand, "Expand: ", opts.Expand)
	logger.TestLog(opts.MatchCase, "MatchCase: ", opts.MatchCase)
	logger.TestLog(opts.All, "All: ", opts.All)
	logger.TestLog(opts.Custom, "Custom: ", opts.Custom)
	logger.TestLog(opts.Prefund, "Prefund: ", opts.Prefund)
	logger.TestLog(opts.Addr, "Addr: ", opts.Addr)
	logger.TestLog(opts.Tags, "Tags: ", opts.Tags)
	logger.TestLog(opts.Clean, "Clean: ", opts.Clean)
	logger.TestLog(opts.Regular, "Regular: ", opts.Regular)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(opts.DryRun, "DryRun: ", opts.DryRun)
	logger.TestLog(len(opts.Autoname) > 0, "Autoname: ", opts.Autoname)
	logger.TestLog(opts.Create, "Create: ", opts.Create)
	logger.TestLog(opts.Update, "Update: ", opts.Update)
	logger.TestLog(opts.Delete, "Delete: ", opts.Delete)
	logger.TestLog(opts.Undelete, "Undelete: ", opts.Undelete)
	logger.TestLog(opts.Remove, "Remove: ", opts.Remove)
	opts.Conn.TestLog()
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *NamesOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// namesFinishParseApi finishes the parsing for server invocations. Returns a new NamesOptions.
func namesFinishParseApi(w http.ResponseWriter, r *http.Request) *NamesOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return NamesFinishParseInternal(w, values)
}

func NamesFinishParseInternal(w io.Writer, values url.Values) *NamesOptions {
	copy := defaultNamesOptions
	copy.Globals.Caps = getCaps()
	opts := &copy
	for key, value := range values {
		switch key {
		case "terms":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Terms = append(opts.Terms, s...)
			}
		case "expand":
			opts.Expand = true
		case "matchCase":
			opts.MatchCase = true
		case "all":
			opts.All = true
		case "custom":
			opts.Custom = true
		case "prefund":
			opts.Prefund = true
		case "addr":
			opts.Addr = true
		case "tags":
			opts.Tags = true
		case "clean":
			opts.Clean = true
		case "regular":
			opts.Regular = true
		case "count":
			opts.Count = true
		case "dryRun":
			opts.DryRun = true
		case "autoname":
			opts.Autoname = value[0]
		case "create":
			opts.Create = true
		case "update":
			opts.Update = true
		case "delete":
			opts.Delete = true
		case "undelete":
			opts.Undelete = true
		case "remove":
			opts.Remove = true
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "names")
				if opts.BadFlag == nil || opts.BadFlag.Error() > err.Error() {
					opts.BadFlag = err
				}
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())
	opts.Autoname, _ = opts.Conn.GetEnsAddress(opts.Autoname)
	opts.AutonameAddr = base.HexToAddress(opts.Autoname)

	// EXISTING_CODE
	opts.OrigTerms = opts.Terms
	opts.Terms, _ = opts.Conn.GetEnsAddresses(opts.Terms)
	// EXISTING_CODE

	return opts
}

// namesFinishParse finishes the parsing for command line invocations. Returns a new NamesOptions.
func namesFinishParse(args []string) *NamesOptions {
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
	opts.Autoname, _ = opts.Conn.GetEnsAddress(opts.Autoname)
	opts.AutonameAddr = base.HexToAddress(opts.Autoname)

	// EXISTING_CODE
	opts.Terms = append(opts.Terms, args...)
	opts.OrigTerms = opts.Terms
	opts.Terms, _ = opts.Conn.GetEnsAddresses(opts.Terms)
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *NamesOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultNamesOptions
}

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra names
	capabilities = capabilities.Add(caps.Default)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	opts := NamesOptions{}
	globals.SetDefaults(&opts.Globals)
	opts.Globals.TestMode = testMode
	opts.Globals.Writer = w
	opts.Globals.Caps = getCaps()
	// EXISTING_CODE
	// EXISTING_CODE
	defaultNamesOptions = opts
}

func (opts *NamesOptions) getCaches() (caches map[walk.CacheType]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
