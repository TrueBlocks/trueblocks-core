// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package slurpPkg

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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc/provider"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/walk"
	// EXISTING_CODE
)

// SlurpOptions provides all command options for the chifra slurp command.
type SlurpOptions struct {
	Addrs       []string                 `json:"addrs,omitempty"`       // One or more addresses to slurp from Etherscan
	Blocks      []string                 `json:"blocks,omitempty"`      // An optional range of blocks to slurp
	BlockIds    []identifiers.Identifier `json:"blockIds,omitempty"`    // Block identifiers
	Parts       []string                 `json:"parts,omitempty"`       // Which types of transactions to request
	Appearances bool                     `json:"appearances,omitempty"` // Show only the blocknumber.tx_id appearances of the exported transactions
	Articulate  bool                     `json:"articulate,omitempty"`  // Articulate the retrieved data if ABIs can be found
	Source      string                   `json:"source,omitempty"`      // The source of the slurped data
	Count       bool                     `json:"count,omitempty"`       // For --appearances mode only, display only the count of records
	Page        uint64                   `json:"page,omitempty"`        // The page to retrieve (page number)
	PageId      string                   `json:"pageId,omitempty"`      // The page to retrieve (page ID)
	PerPage     uint64                   `json:"perPage,omitempty"`     // The number of records to request on each page
	Sleep       float64                  `json:"sleep,omitempty"`       // Seconds to sleep between requests
	Globals     globals.GlobalOptions    `json:"globals,omitempty"`     // The global options
	Conn        *rpc.Connection          `json:"conn,omitempty"`        // The connection to the RPC server
	BadFlag     error                    `json:"badFlag,omitempty"`     // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultSlurpOptions = SlurpOptions{
	Source:  "etherscan",
	PerPage: 1000,
	Sleep:   .25,
}

// testLog is used only during testing to export the options for this test case.
func (opts *SlurpOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Parts) > 0, "Parts: ", opts.Parts)
	logger.TestLog(opts.Appearances, "Appearances: ", opts.Appearances)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	logger.TestLog(len(opts.Source) > 0 && opts.Source != "etherscan", "Source: ", opts.Source)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(opts.Page != 0, "Page: ", opts.Page)
	logger.TestLog(len(opts.PageId) > 0, "PageId: ", opts.PageId)
	logger.TestLog(opts.PerPage != 1000, "PerPage: ", opts.PerPage)
	logger.TestLog(opts.Sleep != float64(.25), "Sleep: ", opts.Sleep)
	opts.Conn.TestLog()
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *SlurpOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// slurpFinishParseApi finishes the parsing for server invocations. Returns a new SlurpOptions.
func slurpFinishParseApi(w http.ResponseWriter, r *http.Request) *SlurpOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return SlurpFinishParseInternal(w, values)
}

func SlurpFinishParseInternal(w io.Writer, values url.Values) *SlurpOptions {
	copy := defaultSlurpOptions
	copy.Globals.Caps = getCaps()
	opts := &copy
	opts.Source = "etherscan"
	opts.PerPage = 1000
	opts.Sleep = .25
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
		case "appearances":
			opts.Appearances = true
		case "articulate":
			opts.Articulate = true
		case "source":
			opts.Source = value[0]
		case "count":
			opts.Count = true
		case "page":
			opts.Page = base.MustParseUint64(value[0])
		case "pageId":
			opts.PageId = value[0]
		case "perPage":
			opts.PerPage = base.MustParseUint64(value[0])
		case "sleep":
			opts.Sleep = base.MustParseFloat64(value[0])
		default:
			if !copy.Globals.Caps.HasKey(key) {
				err := validate.Usage("Invalid key ({0}) in {1} route.", key, "slurp")
				if opts.BadFlag == nil || opts.BadFlag.Error() > err.Error() {
					opts.BadFlag = err
				}
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())

	// EXISTING_CODE
	for _, t := range opts.Parts {
		if t == "all" {
			opts.Parts = []string{"ext", "int", "token", "nfts", "1155", "miner", "uncles", "withdrawals"}
			break
		}
	}
	if len(opts.Parts) == 0 {
		opts.Parts = []string{"ext"}
	}
	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(opts.Addrs)

	return opts
}

// slurpFinishParse finishes the parsing for command line invocations. Returns a new SlurpOptions.
func slurpFinishParse(args []string) *SlurpOptions {
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
	for _, arg := range args {
		if base.IsValidAddress(arg) {
			opts.Addrs = append(opts.Addrs, arg)
		} else {
			opts.Blocks = append(opts.Blocks, arg)
		}
	}
	for _, t := range opts.Parts {
		if t == "all" {
			opts.Parts = []string{"ext", "int", "token", "nfts", "1155", "miner", "uncles", "withdrawals"}
			break
		}
	}
	if len(opts.Parts) == 0 {
		opts.Parts = []string{"ext"}
	}
	// EXISTING_CODE
	opts.Addrs, _ = opts.Conn.GetEnsAddresses(opts.Addrs)
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *SlurpOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultSlurpOptions
}

func getCaps() caps.Capability {
	var capabilities caps.Capability // capabilities for chifra slurp
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Ether)
	capabilities = capabilities.Add(caps.Names)
	// EXISTING_CODE
	// EXISTING_CODE
	return capabilities
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	opts := SlurpOptions{}
	globals.SetDefaults(&opts.Globals)
	opts.Globals.TestMode = testMode
	opts.Globals.Writer = w
	opts.Globals.Caps = getCaps()
	// EXISTING_CODE
	// EXISTING_CODE
	opts.Source = "etherscan"
	opts.PerPage = 1000
	opts.Sleep = .25
	defaultSlurpOptions = opts
}

func (opts *SlurpOptions) getCaches() (caches map[walk.CacheType]bool) {
	// EXISTING_CODE
	caches = map[walk.CacheType]bool{
		walk.Cache_Transactions: true,
		walk.Cache_Abis:         opts.Articulate,
	}
	// EXISTING_CODE
	return
}

// EXISTING_CODE
func (opts *SlurpOptions) Addresses() []base.Address {
	addresses := make([]base.Address, 0, len(opts.Addrs))
	for _, addr := range opts.Addrs {
		addresses = append(addresses, base.HexToAddress(addr))
	}
	return addresses
}

func (opts *SlurpOptions) Query() *provider.Query {
	return &provider.Query{
		Addresses:   opts.Addresses(),
		Resources:   opts.Parts,
		PerPage:     uint(opts.PerPage),
		StartPage:   uint(opts.Page),
		StartPageId: opts.PageId,
		BlockRange:  opts.BlockIds,
	}
}

// Provider returns 3rd party RPC provider based on --source
func (opts *SlurpOptions) Provider() (provider.Provider, error) {
	switch opts.Source {
	case "key":
		return provider.NewKeyProvider(opts.Conn, opts.Globals.Chain)
	case "covalent":
		return provider.NewCovalentProvider(opts.Conn, opts.Globals.Chain)
	case "alchemy":
		return provider.NewAlchemyProvider(opts.Conn, opts.Globals.Chain)
	case "etherscan":
		fallthrough
	default:
		return provider.NewEtherscanProvider(opts.Conn)
	}
}

// EXISTING_CODE
