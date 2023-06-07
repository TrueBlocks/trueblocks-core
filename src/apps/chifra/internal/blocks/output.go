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

	// EXISTING_CODE
	if opts.IsPorted() {
		handled = true
		if opts.Decache {
			err = opts.HandleDecache()

		} else if opts.Count {
			err = opts.HandleCounts()

		} else if opts.Traces {
			err = opts.HandleTrace()

		} else if opts.List > 0 {
			err = opts.HandleList()

		} else if opts.Uncles {
			err = opts.HandleShowUncles()

		} else if opts.Apps || opts.Uniq {
			err = opts.HandleUniq()

		} else {
			err = opts.HandleShowBlocks()
		}

	} else {
		if opts.Globals.IsApiMode() {
			return nil, false
		}

		handled = true
		err = opts.Globals.PassItOn("getBlocks", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStr())
		// TODO: BOGUS -- this is a hack to prevent the output from being written twice. It will be
		// TODO: removed when the etnire command is ported
		opts.Globals.Writer = nil
	}
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

func (opts *BlocksOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	if opts.Decache || opts.Apps || opts.Uniq {
		ported = true
	} else if opts.Cache {
		ported = false
	} else {
		if opts.Count {
			ported = (!opts.Apps && !opts.Uniq)
		} else {
			ported = !opts.Uncles && !opts.Logs && !opts.Apps && !opts.Uniq && !opts.Traces
		}
	}
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
