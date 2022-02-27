// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package blazePkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunBlaze(cmd *cobra.Command, args []string) error {
	opts := BlazeFinishParse(args)

	err := opts.ValidateBlaze()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	opts.ScrapeBlocks()
	return nil
	// EXISTING_CODE
}

func ServeBlaze(w http.ResponseWriter, r *http.Request) bool {
	opts := FromRequest(w, r)

	err := opts.ValidateBlaze()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
