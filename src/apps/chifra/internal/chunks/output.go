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

	if opts.Check {
		err = opts.HandleChunksCheck()

	} else {
		if opts.Extract == "blooms" {
			err = opts.HandleChunksExtract(opts.showBloom)

		} else if opts.Extract == "pins" {
			err = opts.HandleChunksExtractPins()

		} else if opts.Extract == "stats" {
			err := opts.Globals.RenderHeader(ChunkStats{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
			defer opts.Globals.RenderFooter(opts.Globals.ApiMode || opts.Globals.Format == "api")
			if err != nil {
				return err, true
			}
			err = opts.HandleChunksExtract(opts.showStats)
			if err != nil {
				return err, true
			}

		} else {
			err = validate.Usage("Extractor for {0} not yet implemented.", opts.Extract)

		}
	}
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
