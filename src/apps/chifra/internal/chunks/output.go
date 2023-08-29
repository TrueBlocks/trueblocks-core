// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package chunksPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunChunks handles the chunks command for the command line. Returns error only as per cobra.
func RunChunks(cmd *cobra.Command, args []string) (err error) {
	opts := chunksFinishParse(args)
	outputHelpers.SetEnabledForCmds("chunks", opts.IsPorted())
	outputHelpers.SetWriterForCommand("chunks", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.ChunksInternal()
	return
}

// ServeChunks handles the chunks command for the API. Returns error and a bool if handled
func ServeChunks(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := chunksFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("chunks", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("chunks", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.ChunksInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("chunks", err, &opts.Globals)
	return
}

// ChunksInternal handles the internal workings of the chunks command.  Returns error and a bool if handled
func (opts *ChunksOptions) ChunksInternal() (err error, handled bool) {
	err = opts.validateChunks()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra chunks"
	// EXISTING_CODE
	if !opts.IsPorted() {
		logger.Fatal("Should not happen in NamesInternal")
	}

	handled = true

	chain := opts.Globals.Chain
	blockNums, err := identifiers.GetBlockNumbers(chain, opts.BlockIds)
	if err != nil {
		return
	}
	if opts.Globals.TestMode && len(blockNums) > 200 {
		blockNums = blockNums[:200]
	}

	if opts.Pin {
		err = opts.HandlePin(blockNums)

	} else if opts.Publish {
		err = opts.HandlePublish(blockNums)

	} else if opts.Truncate != utils.NOPOS {
		err = opts.HandleTruncate(blockNums)

	} else if opts.Check {
		err = opts.HandleCheck(blockNums)

	} else {
		switch opts.Mode {
		case "manifest":
			err = opts.HandleManifest(blockNums)

		case "index":
			err = opts.HandleIndex(blockNums)

		case "blooms":
			err = opts.HandleBlooms(blockNums)

		case "addresses":
			err = opts.HandleAddresses(blockNums)

		case "appearances":
			err = opts.HandleAppearances(blockNums)

		case "stats":
			err = opts.HandleStats(blockNums)

		default:
			logger.Fatal("Should not happen in NamesInternal")
		}
	}
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetChunksOptions returns the options for this tool so other tools may use it.
func GetChunksOptions(args []string, g *globals.GlobalOptions) *ChunksOptions {
	ret := chunksFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *ChunksOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
func (opts *ChunksOptions) shouldShow(obj index.AddressRecord) bool {
	if opts.Mode == "addresses" || opts.Mode == "appearances" {
		return opts.Globals.Verbose
	}

	for _, addr := range opts.Belongs {
		if hexutil.Encode(obj.Address.Bytes()) == addr {
			return true
		}
	}
	return false
}

// EXISTING_CODE
