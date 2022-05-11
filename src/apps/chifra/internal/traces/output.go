// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package tracesPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunTraces(cmd *cobra.Command, args []string) error {
	opts := TracesFinishParse(args)

	err := opts.ValidateTraces()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	return opts.Globals.PassItOn("getTraces", opts.ToCmdLine())
	// EXISTING_CODE
}

func ServeTraces(w http.ResponseWriter, r *http.Request) bool {
	opts := TracesFinishParseApi(w, r)

	err := opts.ValidateTraces()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	// opts.Globals.PassItOn("getTraces", opts.ToCmdLine())
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
