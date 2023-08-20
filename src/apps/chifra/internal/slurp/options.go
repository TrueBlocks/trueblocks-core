// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package slurpPkg

import (
	"encoding/json"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// SlurpOptions provides all command options for the chifra slurp command.
type SlurpOptions struct {
	Addrs       []string                 `json:"addrs,omitempty"`       // One or more addresses to slurp from Etherscan
	Blocks      []string                 `json:"blocks,omitempty"`      // An optional range of blocks to slurp
	BlockIds    []identifiers.Identifier `json:"blockIds,omitempty"`    // Block identifiers
	Types       []string                 `json:"types,omitempty"`       // Which types of transactions to request
	Appearances bool                     `json:"appearances,omitempty"` // Show only the blocknumber.tx_id appearances of the exported transactions
	PerPage     uint64                   `json:"perPage,omitempty"`     // The number of records to request on each page
	Sleep       float64                  `json:"sleep,omitempty"`       // Seconds to sleep between requests
	Globals     globals.GlobalOptions    `json:"globals,omitempty"`     // The global options
	Conn        *rpc.Connection          `json:"conn,omitempty"`        // The connection to the RPC server
	BadFlag     error                    `json:"badFlag,omitempty"`     // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultSlurpOptions = SlurpOptions{
	PerPage: 5000,
}

// testLog is used only during testing to export the options for this test case.
func (opts *SlurpOptions) testLog() {
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Types) > 0, "Types: ", opts.Types)
	logger.TestLog(opts.Appearances, "Appearances: ", opts.Appearances)
	logger.TestLog(opts.PerPage != 5000, "PerPage: ", opts.PerPage)
	logger.TestLog(opts.Sleep != float64(.25), "Sleep: ", opts.Sleep)
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *SlurpOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// slurpFinishParseApi finishes the parsing for server invocations. Returns a new SlurpOptions.
func slurpFinishParseApi(w http.ResponseWriter, r *http.Request) *SlurpOptions {
	copy := defaultSlurpOptions
	opts := &copy
	opts.PerPage = 5000
	opts.Sleep = .25
	for key, value := range r.URL.Query() {
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
		case "types":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Types = append(opts.Types, s...)
			}
		case "appearances":
			opts.Appearances = true
		case "perPage":
			opts.PerPage = globals.ToUint64(value[0])
		case "sleep":
			opts.Sleep = globals.ToFloat64(value[0])
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "slurp")
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, r, opts.getCaches())

	// EXISTING_CODE
	hasAll := false
	for _, t := range opts.Types {
		if t == "all" {
			hasAll = true
			break
		}
	}
	if hasAll {
		opts.Types = []string{"ext", "int", "token", "nfts", "1155", "miner", "uncles"}
	} else if len(opts.Types) == 0 {
		opts.Types = []string{"ext"}
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
	hasAll := false
	for _, t := range opts.Types {
		if t == "all" {
			hasAll = true
			break
		}
	}
	if hasAll {
		opts.Types = []string{"ext", "int", "token", "nfts", "1155", "miner", "uncles"}
	} else if len(opts.Types) == 0 {
		opts.Types = []string{"ext"}
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

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultSlurpOptions = SlurpOptions{}
	globals.SetDefaults(&defaultSlurpOptions.Globals)
	defaultSlurpOptions.Globals.Writer = w
	capabilities := caps.Default // Additional global caps for chifra slurp
	// EXISTING_CODE
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Raw)
	// EXISTING_CODE
	defaultSlurpOptions.Globals.Caps = capabilities
}

func (opts *SlurpOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	m = map[string]bool{
		"transactions": true,
	}
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
