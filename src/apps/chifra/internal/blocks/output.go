// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package blocksPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunBlocks handles the blocks command for the command line. Returns error only as per cobra.
func RunBlocks(cmd *cobra.Command, args []string) error {
	_ = cmd
	opts := blocksFinishParse(args)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("blocks", &opts.Globals)
	return opts.BlocksInternal(rCtx)
}

// ServeBlocks handles the blocks command for the API. Returns an error.
func ServeBlocks(w http.ResponseWriter, r *http.Request) error {
	opts := blocksFinishParseApi(w, r)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("blocks", w, &opts.Globals)
	err := opts.BlocksInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("blocks", err, &opts.Globals)
	return err
}

// BlocksInternal handles the internal workings of the blocks command. Returns an error.
func (opts *BlocksOptions) BlocksInternal(rCtx *output.RenderCtx) error {
	var err error
	if err = opts.validateBlocks(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra blocks"
	// EXISTING_CODE
	// EXISTING_CODE
	if opts.Globals.Decache {
		err = opts.HandleDecache(rCtx)
	} else if opts.Count {
		err = opts.HandleCount(rCtx)
	} else if opts.Logs {
		err = opts.HandleLogs(rCtx)
	} else if opts.Withdrawals {
		err = opts.HandleWithdrawals(rCtx)
	} else if opts.Traces {
		err = opts.HandleTraces(rCtx)
	} else if opts.Uncles {
		err = opts.HandleUncles(rCtx)
	} else if opts.Uniq {
		err = opts.HandleUniq(rCtx)
	} else if opts.Hashes {
		err = opts.HandleHashes(rCtx)
	} else {
		err = opts.HandleShow(rCtx)
	}
	timer.Report(msg)

	return err
}

// GetBlocksOptions returns the options for this tool so other tools may use it.
func GetBlocksOptions(args []string, g *globals.GlobalOptions) *BlocksOptions {
	ret := blocksFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
