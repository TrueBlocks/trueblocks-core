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
	"log"
	"net/http"

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
	if opts.Ported() {
		handled = true
		if opts.List > 0 {
			err = opts.HandleList()

		} else if opts.Globals.ShowRaw {
			err = opts.HandleShowBlocks()

		} else {
			log.Panic("Invalid chifra blocks: unhandled case")
		}

	} else {
		if opts.Globals.IsApiMode() {
			return nil, false
		}

		handled = true
		err = opts.Globals.PassItOn("getBlocks", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStr())
	}
	// EXISTING_CODE

	return
}

// EXISTING_CODE
func (opts *BlocksOptions) Ported() bool {
	// return opts.Globals.ShowRaw || opts.List > 0
	// return opts.Globals.ShowRaw
	// return opts.List > 0
	return false
}

// EXISTING_CODE
