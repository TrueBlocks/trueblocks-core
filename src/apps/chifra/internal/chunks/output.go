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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunChunks(cmd *cobra.Command, args []string) error {
	opts := ChunksFinishParse(args)

	err := opts.ValidateChunks()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	if opts.Check {
		return opts.HandleChunksCheck()
	}

	if opts.Extract == "blooms" {
		return opts.HandleChunksExtract(opts.showBloom)
	} else if opts.Extract == "pins" {
		return opts.HandleChunksExtractPins()
	} else if opts.Extract == "stats" {
		return opts.HandleChunksExtract(opts.showStats)
	}

	return validate.Usage("Extractor for {0} not yet implemented.", opts.Extract)
	// EXISTING_CODE
}

func ServeChunks(w http.ResponseWriter, r *http.Request) bool {
	opts := ChunksFinishParseApi(w, r)

	err := opts.ValidateChunks()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	if opts.Check {
		err = opts.HandleChunksCheck()
		if err != nil {
			logger.Log(logger.Warning, "Could not extract blooms", err)
		}
		return true
	}

	if opts.Extract == "blooms" {
		err = opts.HandleChunksExtract(opts.showBloom)
		if err != nil {
			logger.Log(logger.Warning, "Could not extract blooms", err)
		}
		return true
	} else if opts.Extract == "pins" {
		err = opts.HandleChunksExtractPins()
		if err != nil {
			logger.Log(logger.Warning, "Could not extract pin list", err)
		}
		return true
	} else if opts.Extract == "stats" {
		err = opts.HandleChunksExtract(opts.showStats)
		if err != nil {
			logger.Log(logger.Warning, "Could not extract stats", err)
		}
		return true
	}

	opts.Globals.RespondWithError(w, http.StatusInternalServerError, validate.Usage("Extractor for {0} not yet implemented.", opts.Extract))
	return true
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
