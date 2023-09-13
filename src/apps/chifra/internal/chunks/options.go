// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package chunksPkg

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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// ChunksOptions provides all command options for the chifra chunks command.
type ChunksOptions struct {
	Mode       string                   `json:"mode,omitempty"`       // The type of data to process
	Blocks     []string                 `json:"blocks,omitempty"`     // An optional list of blocks to intersect with chunk ranges
	BlockIds   []identifiers.Identifier `json:"blockIds,omitempty"`   // Block identifiers
	Check      bool                     `json:"check,omitempty"`      // Check the manifest, index, or blooms for internal consistency
	Pin        bool                     `json:"pin,omitempty"`        // Pin the manifest or each index chunk and bloom
	Publish    bool                     `json:"publish,omitempty"`    // Publish the manifest to the Unchained Index smart contract
	Publisher  string                   `json:"publisher,omitempty"`  // For some query options, the publisher of the index
	Truncate   uint64                   `json:"truncate,omitempty"`   // Truncate the entire index at this block (requires a block identifier)
	Remote     bool                     `json:"remote,omitempty"`     // Prior to processing, retreive the manifest from the Unchained Index smart contract
	Belongs    []string                 `json:"belongs,omitempty"`    // In index mode only, checks the address(es) for inclusion in the given index chunk
	FirstBlock uint64                   `json:"firstBlock,omitempty"` // First block to process (inclusive)
	LastBlock  uint64                   `json:"lastBlock,omitempty"`  // Last block to process (inclusive)
	MaxAddrs   uint64                   `json:"maxAddrs,omitempty"`   // The max number of addresses to process in a given chunk
	Deep       bool                     `json:"deep,omitempty"`       // If true, dig more deeply during checking (manifest only)
	Sleep      float64                  `json:"sleep,omitempty"`      // For --remote pinning only, seconds to sleep between API calls
	Globals    globals.GlobalOptions    `json:"globals,omitempty"`    // The global options
	Conn       *rpc.Connection          `json:"conn,omitempty"`       // The connection to the RPC server
	BadFlag    error                    `json:"badFlag,omitempty"`    // An error flag if needed
	// EXISTING_CODE
	// EXISTING_CODE
}

var defaultChunksOptions = ChunksOptions{
	Publisher: "trueblocks.eth",
	Truncate:  utils.NOPOS,
	LastBlock: utils.NOPOS,
	MaxAddrs:  utils.NOPOS,
}

// testLog is used only during testing to export the options for this test case.
func (opts *ChunksOptions) testLog() {
	logger.TestLog(len(opts.Mode) > 0, "Mode: ", opts.Mode)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(opts.Check, "Check: ", opts.Check)
	logger.TestLog(opts.Pin, "Pin: ", opts.Pin)
	logger.TestLog(opts.Publish, "Publish: ", opts.Publish)
	logger.TestLog(len(opts.Publisher) > 0 && opts.Publisher != "trueblocks.eth", "Publisher: ", opts.Publisher)
	logger.TestLog(opts.Truncate != utils.NOPOS, "Truncate: ", opts.Truncate)
	logger.TestLog(opts.Remote, "Remote: ", opts.Remote)
	logger.TestLog(len(opts.Belongs) > 0, "Belongs: ", opts.Belongs)
	logger.TestLog(opts.FirstBlock != 0, "FirstBlock: ", opts.FirstBlock)
	logger.TestLog(opts.LastBlock != 0 && opts.LastBlock != utils.NOPOS, "LastBlock: ", opts.LastBlock)
	logger.TestLog(opts.MaxAddrs != utils.NOPOS, "MaxAddrs: ", opts.MaxAddrs)
	logger.TestLog(opts.Deep, "Deep: ", opts.Deep)
	logger.TestLog(opts.Sleep != float64(0.0), "Sleep: ", opts.Sleep)
	opts.Conn.TestLog(opts.getCaches())
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ChunksOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "  ")
	return string(b)
}

// chunksFinishParseApi finishes the parsing for server invocations. Returns a new ChunksOptions.
func chunksFinishParseApi(w http.ResponseWriter, r *http.Request) *ChunksOptions {
	copy := defaultChunksOptions
	opts := &copy
	opts.Truncate = utils.NOPOS
	opts.FirstBlock = 0
	opts.LastBlock = utils.NOPOS
	opts.MaxAddrs = utils.NOPOS
	opts.Sleep = 0.0
	for key, value := range r.URL.Query() {
		switch key {
		case "mode":
			opts.Mode = value[0]
		case "blocks":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Blocks = append(opts.Blocks, s...)
			}
		case "check":
			opts.Check = true
		case "pin":
			opts.Pin = true
		case "publish":
			opts.Publish = true
		case "publisher":
			opts.Publisher = value[0]
		case "truncate":
			opts.Truncate = globals.ToUint64(value[0])
		case "remote":
			opts.Remote = true
		case "belongs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Belongs = append(opts.Belongs, s...)
			}
		case "firstBlock":
			opts.FirstBlock = globals.ToUint64(value[0])
		case "lastBlock":
			opts.LastBlock = globals.ToUint64(value[0])
		case "maxAddrs":
			opts.MaxAddrs = globals.ToUint64(value[0])
		case "deep":
			opts.Deep = true
		case "sleep":
			opts.Sleep = globals.ToFloat64(value[0])
		default:
			if !copy.Globals.Caps.HasKey(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "chunks")
			}
		}
	}
	opts.Conn = opts.Globals.FinishParseApi(w, r, opts.getCaches())
	opts.Publisher, _ = opts.Conn.GetEnsAddress(opts.Publisher)

	// EXISTING_CODE
	// EXISTING_CODE
	opts.Belongs, _ = opts.Conn.GetEnsAddresses(opts.Belongs)

	return opts
}

// chunksFinishParse finishes the parsing for command line invocations. Returns a new ChunksOptions.
func chunksFinishParse(args []string) *ChunksOptions {
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
	opts.Publisher, _ = opts.Conn.GetEnsAddress(opts.Publisher)

	// EXISTING_CODE
	if len(args) > 0 {
		opts.Mode = args[0]
		for i, arg := range args {
			if i > 0 {
				if base.IsValidAddress(arg) {
					opts.Belongs = append(opts.Belongs, arg)
				} else {
					opts.Blocks = append(opts.Blocks, arg)
				}
			}
		}
	}
	if opts.Truncate == 0 {
		opts.Truncate = utils.NOPOS
	}
	if opts.LastBlock == 0 {
		opts.LastBlock = utils.NOPOS
	}
	if opts.MaxAddrs == 0 {
		opts.MaxAddrs = utils.NOPOS
	}
	getDef := func(def string) string {
		if (opts.Mode == "index" && opts.Check) ||
			(opts.Mode == "manifest" && opts.Check) ||
			opts.Truncate != utils.NOPOS || len(opts.Belongs) > 0 {
			return "json"
		}
		return def
	}
	defFmt = getDef(defFmt)
	// EXISTING_CODE
	opts.Belongs, _ = opts.Conn.GetEnsAddresses(opts.Belongs)
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}

	return opts
}

func GetOptions() *ChunksOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &defaultChunksOptions
}

func ResetOptions() {
	// We want to keep writer between command file calls
	w := GetOptions().Globals.Writer
	defaultChunksOptions = ChunksOptions{}
	globals.SetDefaults(&defaultChunksOptions.Globals)
	defaultChunksOptions.Globals.Writer = w
	capabilities := caps.Default // Additional global caps for chifra chunks
	// EXISTING_CODE
	// EXISTING_CODE
	defaultChunksOptions.Globals.Caps = capabilities
}

func (opts *ChunksOptions) getCaches() (m map[string]bool) {
	// EXISTING_CODE
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
