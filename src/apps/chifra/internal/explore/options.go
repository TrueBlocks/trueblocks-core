// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package explorePkg

import (
	// EXISTING_CODE
	"encoding/json"
	"io"
	"net/http"
	"net/url"
	"path"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
	// EXISTING_CODE
)

// ExploreOptions provides all command options for the chifra explore command.
type ExploreOptions struct {
	Terms   []string              `json:"terms,omitempty"`   // One or more address, name, block, or transaction identifier
	Local   bool                  `json:"local,omitempty"`   // Open the local TrueBlocks explorer
	Google  bool                  `json:"google,omitempty"`  // Search google excluding popular blockchain explorers
	Dalle   bool                  `json:"dalle,omitempty"`   // Open the address to the DalleDress explorer
	Globals globals.GlobalOptions `json:"globals,omitempty"` // The global options
	Conn    *rpc.Connection       `json:"conn,omitempty"`    // The connection to the RPC server
	BadFlag error                 `json:"badFlag,omitempty"` // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultExploreOptions = ExploreOptions{}

// testLog is used only during testing to export the options for this test case.
func (opts *ExploreOptions) testLog() {
	logger.TestLog(len(opts.Terms) > 0, "Terms: ", opts.Terms)
	logger.TestLog(opts.Local, "Local: ", opts.Local)
	logger.TestLog(opts.Google, "Google: ", opts.Google)
	logger.TestLog(opts.Dalle, "Dalle: ", opts.Dalle)
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ExploreOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// exploreFinishParseApi finishes the parsing for server invocations. Returns a new ExploreOptions.
func exploreFinishParseApi(w http.ResponseWriter, r *http.Request) *ExploreOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return ExploreFinishParseInternal(w, values)
}

func ExploreFinishParseInternal(w io.Writer, values url.Values) *ExploreOptions {
	copy := defaultExploreOptions
	copy.Globals.Caps = getCaps()
	opts := &copy
	for key, value := range values {
		switch key {
		case "terms":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Terms = append(opts.Terms, s...)
			}
		case "local":
			opts.Local = true
		case "google":
			opts.Google = true
		case "dalle":
			opts.Dalle = true
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "explore")
				if opts.BadFlag == nil || opts.BadFlag.Error() > err.Error() {
					opts.BadFlag = err
				}
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())

	// EXISTING_CODE
	opts.Terms, _ = opts.Conn.GetEnsAddresses(opts.Terms)
	// EXISTING_CODE

	return opts
}

// exploreFinishParse finishes the parsing for command line invocations. Returns a new ExploreOptions.
func exploreFinishParse(args []string) *ExploreOptions {
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
	opts.Terms = append(opts.Terms, args...)
	opts.Terms, _ = opts.Conn.GetEnsAddresses(opts.Terms)
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *ExploreOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultExploreOptions
}

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra explore
	capabilities = capabilities.Add(caps.Verbose)
	capabilities = capabilities.Add(caps.Version)
	capabilities = capabilities.Add(caps.Noop)
	capabilities = capabilities.Add(caps.NoColor)
	capabilities = capabilities.Add(caps.Chain)
	capabilities = capabilities.Add(caps.File)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	opts := ExploreOptions{}
	globals.SetDefaults(&opts.Globals)
	opts.Globals.TestMode = testMode
	opts.Globals.Writer = w
	opts.Globals.Caps = getCaps()
	defaultExploreOptions = opts
}

func (opts *ExploreOptions) getCaches() (caches map[walk.CacheType]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
func (u *ExploreUrl) getUrl(opts *ExploreOptions) string {

	var chain = opts.Globals.Chain

	if opts.Google {
		var query = "https://www.google.com/search?q=[{TERM}]"
		query = strings.Replace(query, "[{TERM}]", u.term, -1)
		var exclusions = []string{
			"etherscan", "etherchain", "bloxy", "bitquery", "ethplorer", "tokenview", "anyblocks", "explorer",
		}
		for _, ex := range exclusions {
			query += ("+-" + ex)
		}
		return query
	}

	if opts.Dalle {
		var query = "http://192.34.63.136:8080/dalle/simple/[{TERM}]"
		return strings.Replace(query, "[{TERM}]", u.term, -1)
	}

	if u.termType == ExploreFourByte {
		var query = "https://www.4byte.directory/signatures/?bytes4_signature=[{TERM}]"
		query = strings.Replace(query, "[{TERM}]", u.term, -1)
		return query
	}

	if u.termType == ExploreEnsName {
		var query = "https://app.ens.domains/name/[{TERM}]/details"
		query = strings.Replace(query, "[{TERM}]", u.term, -1)
		return query
	}

	url := config.GetChain(chain).RemoteExplorer
	query := ""
	switch u.termType {
	case ExploreNone:
		// do nothing
	case ExploreTx:
		query = path.Join("tx", u.term)
	case ExploreBlock:
		query = path.Join("block", u.term)
	case ExploreAddress:
		fallthrough
	default:
		query = path.Join("address", u.term)
	}

	if opts.Local {
		url = config.GetChain(chain).LocalExplorer
		query = strings.Replace(query, "tx/", "explorer/transactions/", -1)
		query = strings.Replace(query, "block/", "explorer/blocks/", -1)
	}

	return url + query
}

// EXISTING_CODE
