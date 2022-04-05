// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package exportPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunExport(cmd *cobra.Command, args []string) error {
	opts := ExportFinishParse(args)

	err := opts.ValidateExport()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	err = opts.FreshenMonitors()
	if err != nil {
		return err
	}

	return opts.Globals.PassItOn("acctExport", opts.ToCmdLine())
	// EXISTING_CODE
}

func ServeExport(w http.ResponseWriter, r *http.Request) bool {
	opts := FromRequest(w, r)

	err := opts.ValidateExport()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	err = opts.FreshenMonitors()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// opts.Globals.PassItOn("acctExport", opts.ToCmdLine())
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
