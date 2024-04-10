// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package blocksPkg

import (
	// EXISTING_CODE
	"encoding/json"
	"io"
	"net/http"
	"net/url"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	// EXISTING_CODE
)

// BlocksOptions provides all command options for the chifra blocks command.
type BlocksOptions struct {
	Blocks      []string                 `json:"blocks,omitempty"`      // A space-separated list of one or more block identifiers
	BlockIds    []identifiers.Identifier `json:"blockIds,omitempty"`    // Block identifiers
	Hashes      bool                     `json:"hashes,omitempty"`      // Display only transaction hashes, default is to display full transaction detail
	Uncles      bool                     `json:"uncles,omitempty"`      // Display uncle blocks (if any) instead of the requested block
	Traces      bool                     `json:"traces,omitempty"`      // Export the traces from the block as opposed to the block data
	Uniq        bool                     `json:"uniq,omitempty"`        // Display a list of uniq address appearances per transaction
	Flow        string                   `json:"flow,omitempty"`        // For the --uniq option only, export only from or to (including trace from or to)
	Logs        bool                     `json:"logs,omitempty"`        // Display only the logs found in the block(s)
	Emitter     []string                 `json:"emitter,omitempty"`     // For the --logs option only, filter logs to show only those logs emitted by the given address(es)
	Topic       []string                 `json:"topic,omitempty"`       // For the --logs option only, filter logs to show only those with this topic(s)
	Withdrawals bool                     `json:"withdrawals,omitempty"` // Export the withdrawals from the block as opposed to the block data
	Articulate  bool                     `json:"articulate,omitempty"`  // For the --logs option only, articulate the retrieved data if ABIs can be found
	BigRange    uint64                   `json:"bigRange,omitempty"`    // For the --logs option only, allow for block ranges larger than 500
	Count       bool                     `json:"count,omitempty"`       // Display only the count of appearances for --addrs or --uniq
	CacheTxs    bool                     `json:"cacheTxs,omitempty"`    // Force a write of the block's transactions to the cache (slow)
	CacheTraces bool                     `json:"cacheTraces,omitempty"` // Force a write of the block's traces to the cache (slower)
	List        uint64                   `json:"list,omitempty"`        // Summary list of blocks running backwards from latest block minus num
	ListCount   uint64                   `json:"listCount,omitempty"`   // The number of blocks to report for --list option
	Globals     globals.GlobalOptions    `json:"globals,omitempty"`     // The global options
	Conn        *rpc.Connection          `json:"conn,omitempty"`        // The connection to the RPC server
	BadFlag     error                    `json:"badFlag,omitempty"`     // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultBlocksOptions = BlocksOptions{
	BigRange: 500,
}

// testLog is used only during testing to export the options for this test case.
func (opts *BlocksOptions) testLog() {
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(opts.Hashes, "Hashes: ", opts.Hashes)
	logger.TestLog(opts.Uncles, "Uncles: ", opts.Uncles)
	logger.TestLog(opts.Traces, "Traces: ", opts.Traces)
	logger.TestLog(opts.Uniq, "Uniq: ", opts.Uniq)
	logger.TestLog(len(opts.Flow) > 0, "Flow: ", opts.Flow)
	logger.TestLog(opts.Logs, "Logs: ", opts.Logs)
	logger.TestLog(len(opts.Emitter) > 0, "Emitter: ", opts.Emitter)
	logger.TestLog(len(opts.Topic) > 0, "Topic: ", opts.Topic)
	logger.TestLog(opts.Withdrawals, "Withdrawals: ", opts.Withdrawals)
	logger.TestLog(opts.Articulate, "Articulate: ", opts.Articulate)
	logger.TestLog(opts.BigRange != 500, "BigRange: ", opts.BigRange)
	logger.TestLog(opts.Count, "Count: ", opts.Count)
	logger.TestLog(opts.CacheTxs, "CacheTxs: ", opts.CacheTxs)
	logger.TestLog(opts.CacheTraces, "CacheTraces: ", opts.CacheTraces)
	logger.TestLog(opts.List != 0, "List: ", opts.List)
	logger.TestLog(opts.ListCount != 0, "ListCount: ", opts.ListCount)
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *BlocksOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// blocksFinishParseApi finishes the parsing for server invocations. Returns a new BlocksOptions.
func blocksFinishParseApi(w http.ResponseWriter, r *http.Request) *BlocksOptions {
	values := r.URL.Query()
	if r.Header.Get("User-Agent") == "testRunner" {
		values.Set("testRunner", "true")
	}
	return BlocksFinishParseInternal(w, values)
}

func BlocksFinishParseInternal(w io.Writer, values url.Values) *BlocksOptions {
	copy := defaultBlocksOptions
	opts := &copy
	opts.BigRange = 500
	for key, value := range values {
		switch key {
		case "blocks":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Blocks = append(opts.Blocks, s...)
			}
		case "hashes":
			opts.Hashes = true
		case "uncles":
			opts.Uncles = true
		case "traces":
			opts.Traces = true
		case "uniq":
			opts.Uniq = true
		case "flow":
			opts.Flow = value[0]
		case "logs":
			opts.Logs = true
		case "emitter":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Emitter = append(opts.Emitter, s...)
			}
		case "topic":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Topic = append(opts.Topic, s...)
			}
		case "withdrawals":
			opts.Withdrawals = true
		case "articulate":
			opts.Articulate = true
		case "bigRange":
			opts.BigRange = globals.ToUint64(value[0])
		case "count":
			opts.Count = true
		case "cacheTxs":
			opts.CacheTxs = true
		case "cacheTraces":
			opts.CacheTraces = true
		case "list":
			opts.List = globals.ToUint64(value[0])
		case "listCount":
			opts.ListCount = globals.ToUint64(value[0])
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "blocks")
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, values, opts.getCaches())

	// EXISTING_CODE
	// EXISTING_CODE
	opts.Emitter, _ = opts.Conn.GetEnsAddresses(opts.Emitter)

	return opts
}

// blocksFinishParse finishes the parsing for command line invocations. Returns a new BlocksOptions.
func blocksFinishParse(args []string) *BlocksOptions {
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
	opts.Blocks = args
	if !opts.Uniq && opts.List == 0 {
		defFmt = "json"
	}
	// EXISTING_CODE
	opts.Emitter, _ = opts.Conn.GetEnsAddresses(opts.Emitter)
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *BlocksOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultBlocksOptions
}

func ResetOptions(testMode bool) {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultBlocksOptions = BlocksOptions{}
	globals.SetDefaults(&defaultBlocksOptions.Globals)
	defaultBlocksOptions.Globals.TestMode = testMode
	defaultBlocksOptions.Globals.Writer = w
	var capabilities caps.Capability // capabilities for chifra blocks
	capabilities = capabilities.Add(caps.Default)
	capabilities = capabilities.Add(caps.Caching)
	capabilities = capabilities.Add(caps.Ether)
	capabilities = capabilities.Add(caps.Raw)
	// EXISTING_CODE
	// EXISTING_CODE
	defaultBlocksOptions.Globals.Caps = capabilities
}

func (opts *BlocksOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	m = map[string]bool{
		"blocks":       !opts.Uncles,
		"receipts":     !opts.Uncles,
		"transactions": opts.CacheTxs || opts.Uniq,
		"traces":       opts.CacheTraces || (opts.Globals.Cache && (opts.Traces || opts.Uniq)),
		"logs":         opts.Logs,
	}
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
