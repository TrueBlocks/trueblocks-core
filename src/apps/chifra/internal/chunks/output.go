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
	// JINKY
	// JINKY
	err, _ = opts.ChunksInternal()
	return
}

// ServeChunks handles the chunks command for the API. Returns error and a bool if handled
func ServeChunks(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := ChunksFinishParseApi(w, r)
	// JINKY
	// JINKY
	return opts.ChunksInternal()
}

// ChunksInternal handles the internal workings of the chunks command.  Returns error and a bool if handled
func (opts *ChunksOptions) ChunksInternal() (err error, handled bool) {
	err = opts.ValidateChunks()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.Check {
		return opts.HandleChunksCheck(), true
	}

	handled = true
	if opts.Extract == "blooms" {
		err = opts.HandleChunksExtract(opts.showBloom)

	} else if opts.Extract == "pins" {
		err = opts.HandleChunksExtractPins()

	} else if opts.Extract == "stats" {
		err = opts.HandleChunksExtract(opts.showStats)

	} else {
		err = validate.Usage("Extractor for {0} not yet implemented.", opts.Extract)

	}
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
