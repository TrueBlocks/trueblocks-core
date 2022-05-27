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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
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

	if opts.Check {
		return opts.HandleChunksCheck(blockNums), true

	} else if opts.Mode == "blooms" {
		return opts.HandleChunksExtract(opts.showBloom, blockNums), true

	} else if opts.Mode == "index" || opts.Mode == "addresses" || opts.Mode == "appearances" {
		return validate.Usage("Extractor for {0} not yet implemented.", opts.Mode), true

	} else {
		defer opts.Globals.RenderFooter(opts.Globals.ApiMode || opts.Globals.Format == "api")

		if opts.Mode == "pins" {
			opts.PrintManifestHeader()
			err := opts.Globals.RenderHeader(types.SimplePinList{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
			if err != nil {
				return err, true
			}
			return opts.HandleChunksExtractPins(), true

		} else if opts.Mode == "stats" {
			err := opts.Globals.RenderHeader(ChunkStats{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
			if err != nil {
				return err, true
			}
			return opts.HandleChunksExtract(opts.showStats, blockNums), true

		}
	}
	// EXISTING_CODE

	return
}

// EXISTING_CODE
func (opts *ChunksOptions) showStats(path string, first bool) error {
	// TODO: Fix export without arrays
	obj := NewChunkStats(path)
	err := opts.Globals.RenderObject(obj, false, first)
	if err != nil {
		return err
	}
	return nil
}

// EXISTING_CODE
