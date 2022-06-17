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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunChunks handles the chunks command for the command line. Returns error only as per cobra.
func RunChunks(cmd *cobra.Command, args []string) (err error) {
	opts := ChunksFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.ChunksInternal()
	return
}

// ServeChunks handles the chunks command for the API. Returns error and a bool if handled
func ServeChunks(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := ChunksFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	return opts.ChunksInternal()
}

// ChunksInternal handles the internal workings of the chunks command.  Returns error and a bool if handled
func (opts *ChunksOptions) ChunksInternal() (err error, handled bool) {
	err = opts.ValidateChunks()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	handled = true

	blockNums, err := blockRange.GetBlockNumArray(opts.Globals.Chain, opts.BlockIds)
	if opts.Globals.TestMode && len(blockNums) > 200 {
		blockNums = blockNums[:200]
	}
	if err != nil {
		return
	}

	maxTestItems = 100
	if opts.Check || opts.PinChunks || opts.PinData {
		if opts.Check {
			err = opts.HandleChunksCheck(blockNums)
			if err != nil {
				return
			}
		}
		// TODO: BOGUS - Complete this work
		return

	} else {
		switch opts.Mode {
		case "stats":
			err = opts.HandleStats(blockNums)

		case "pins":
			err = opts.HandlePins(blockNums)

		case "manifest":
			if opts.Globals.Format == "txt" || opts.Globals.Format == "csv" {
				err = opts.HandlePins(blockNums)
			} else {
				err = opts.HandleManifest(blockNums)
			}

		case "blooms":
			err = opts.HandleBlooms(blockNums)

		case "index":
			err = opts.HandleIndex(blockNums)

		case "addresses":
			if opts.Belongs {
				err = opts.HandleAddressesBelongs(blockNums)
			} else {
				err = opts.HandleAddresses(blockNums)
			}

		case "appearances":
			err = opts.HandleAppearances(blockNums)

		default:
			err = validate.Usage("Extractor for {0} not yet implemented.", opts.Mode)
		}
	}
	// EXISTING_CODE

	return
}

// EXISTING_CODE
var maxTestItems = 100

// EXISTING_CODE
