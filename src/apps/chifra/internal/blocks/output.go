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
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunBlocks handles the blocks command for the command line. Returns error only as per cobra.
func RunBlocks(cmd *cobra.Command, args []string) (err error) {
	opts := blocksFinishParse(args)
	outputHelpers.SetWriterForCommand("blocks", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.BlocksInternal()
	return
}

// ServeBlocks handles the blocks command for the API. Returns error and a bool if handled
func ServeBlocks(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := blocksFinishParseApi(w, r)
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

	// EXISTING_CODE
	if false && opts.List > 0 {
		// return opts.HandleList(), true
		opts.HandleList()
	}

	if opts.Globals.IsApiMode() {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("getBlocks", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStr())
	// EXISTING_CODE

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

// EXISTING_CODE
// EXISTING_CODE
