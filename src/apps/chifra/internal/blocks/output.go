// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package blocksPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunBlocks handles the blocks command for the command line. Returns error only as per cobra.
func RunBlocks(cmd *cobra.Command, args []string) (err error) {
	opts := blocksFinishParse(args)
	outputHelpers.SetEnabledForCmds("blocks", opts.IsPorted())
	outputHelpers.SetWriterForCommand("blocks", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.BlocksInternal()
	return
}

// ServeBlocks handles the blocks command for the API. Returns error and a bool if handled
func ServeBlocks(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := blocksFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("blocks", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("blocks", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.BlocksInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("blocks", err, &opts.Globals)
	return
}

// BlocksInternal handles the internal workings of the blocks command.  Returns error and a bool if handled
func (opts *BlocksOptions) BlocksInternal() (err error, handled bool) {
	err = opts.validateBlocks()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra blocks"
	// EXISTING_CODE
	if !opts.IsPorted() {
		logger.Fatal("Should not happen in BlocksInternal")
	}

	handled = true
	if opts.Globals.Decache {
		err = opts.HandleDecache()

	} else if opts.Count {
		err = opts.HandleCounts()

	} else if opts.Logs {
		err = opts.HandleLogs()

	} else if opts.Traces {
		err = opts.HandleTraces()

	} else if opts.Uncles {
		err = opts.HandleUncles()

	} else if opts.List > 0 {
		err = opts.HandleList()

	} else if opts.Uniq {
		err = opts.HandleUniq()

	} else if opts.Hashes {
		err = opts.HandleHashes()

	} else {
		err = opts.HandleShow()
	}
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetBlocksOptions returns the options for this tool so other tools may use it.
func GetBlocksOptions(args []string, g *globals.GlobalOptions) *BlocksOptions {
	ret := blocksFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *BlocksOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
