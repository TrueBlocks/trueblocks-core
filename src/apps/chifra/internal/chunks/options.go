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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// ChunksOptions provides all command options for the chifra chunks command.
type ChunksOptions struct {
	Mode      string                   `json:"mode,omitempty"`      // The type of chunk info to retrieve
	Blocks    []string                 `json:"blocks,omitempty"`    // Optional list of blocks to intersect with chunk ranges
	BlockIds  []identifiers.Identifier `json:"blockIds,omitempty"`  // Block identifiers
	Addrs     []string                 `json:"addrs,omitempty"`     // One or more addresses to use with --belongs option (see note)
	Details   bool                     `json:"details,omitempty"`   // For manifest and addresses options only, display full details of the report
	Check     bool                     `json:"check,omitempty"`     // Depends on mode, checks for internal consistency of the data type
	Belongs   bool                     `json:"belongs,omitempty"`   // Checks if the given address appears in the given chunk
	Repair    bool                     `json:"repair,omitempty"`    // Valid for manifest option only, repair the given chunk (requires block number)
	Clean     bool                     `json:"clean,omitempty"`     // Retrieve all pins on Pinata, compare to manifest, remove any extraneous remote pins
	Remote    bool                     `json:"remote,omitempty"`    // For some options, force processing from remote data
	Reset     uint64                   `json:"reset,omitempty"`     // Available only in chunks mode, remove all chunks inclusive of or after this block
	PinRemote bool                     `json:"pinRemote,omitempty"` // Pin any previously unpinned chunks and blooms to a remote pinning service
	Publish   bool                     `json:"publish,omitempty"`   // Update the manifest and publish it to the Unchained Index smart contract
	Globals   globals.GlobalOptions    `json:"globals,omitempty"`   // The global options
	BadFlag   error                    `json:"badFlag,omitempty"`   // An error flag if needed
}

var chunksCmdLineOptions ChunksOptions

// testLog is used only during testing to export the options for this test case.
func (opts *ChunksOptions) testLog() {
	logger.TestLog(len(opts.Mode) > 0, "Mode: ", opts.Mode)
	logger.TestLog(len(opts.Blocks) > 0, "Blocks: ", opts.Blocks)
	logger.TestLog(len(opts.Addrs) > 0, "Addrs: ", opts.Addrs)
	logger.TestLog(opts.Details, "Details: ", opts.Details)
	logger.TestLog(opts.Check, "Check: ", opts.Check)
	logger.TestLog(opts.Belongs, "Belongs: ", opts.Belongs)
	logger.TestLog(opts.Repair, "Repair: ", opts.Repair)
	logger.TestLog(opts.Clean, "Clean: ", opts.Clean)
	logger.TestLog(opts.Remote, "Remote: ", opts.Remote)
	logger.TestLog(opts.Reset != utils.NOPOS, "Reset: ", opts.Reset)
	logger.TestLog(opts.PinRemote, "PinRemote: ", opts.PinRemote)
	logger.TestLog(opts.Publish, "Publish: ", opts.Publish)
	opts.Globals.TestLog()
}

// String implements the Stringer interface
func (opts *ChunksOptions) String() string {
	b, _ := json.MarshalIndent(opts, "", "\t")
	return string(b)
}

// chunksFinishParseApi finishes the parsing for server invocations. Returns a new ChunksOptions.
func chunksFinishParseApi(w http.ResponseWriter, r *http.Request) *ChunksOptions {
	opts := &ChunksOptions{}
	opts.Reset = utils.NOPOS
	for key, value := range r.URL.Query() {
		switch key {
		case "mode":
			opts.Mode = value[0]
		case "blocks":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Blocks = append(opts.Blocks, s...)
			}
		case "addrs":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Addrs = append(opts.Addrs, s...)
			}
		case "details":
			opts.Details = true
		case "check":
			opts.Check = true
		case "belongs":
			opts.Belongs = true
		case "repair":
			opts.Repair = true
		case "clean":
			opts.Clean = true
		case "remote":
			opts.Remote = true
		case "reset":
			opts.Reset = globals.ToUint64(value[0])
		case "pinRemote":
			opts.PinRemote = true
		case "publish":
			opts.Publish = true
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "chunks")
				return opts
			}
		}
	}
	opts.Globals = *globals.GlobalsFinishParseApi(w, r)
	// EXISTING_CODE
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, opts.Addrs)
	// EXISTING_CODE

	return opts
}

// chunksFinishParse finishes the parsing for command line invocations. Returns a new ChunksOptions.
func chunksFinishParse(args []string) *ChunksOptions {
	opts := GetOptions()
	opts.Globals.FinishParse(args)
	defFmt := "txt"
	// EXISTING_CODE
	if len(args) > 0 {
		opts.Mode = args[0]
		for i, arg := range args {
			if i > 0 {
				if validate.IsValidAddress(arg) {
					opts.Addrs = append(opts.Addrs, arg)
				} else {
					opts.Blocks = append(opts.Blocks, arg)
				}
			}
		}
	}
	opts.Addrs = ens.ConvertEns(opts.Globals.Chain, opts.Addrs)
	if opts.Mode == "manifest" {
		defFmt = "json"
	}
	if opts.Reset == 0 {
		opts.Reset = utils.NOPOS
	}
	// EXISTING_CODE
	if len(opts.Globals.Format) == 0 || opts.Globals.Format == "none" {
		opts.Globals.Format = defFmt
	}
	return opts
}

func GetOptions() *ChunksOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &chunksCmdLineOptions
}
