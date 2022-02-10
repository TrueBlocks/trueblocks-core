// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package monitorsPkg

// EXISTING_CODE
import (
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunMonitors(cmd *cobra.Command, args []string) error {
	opts := MonitorsFinishParse(args)

	err := opts.ValidateMonitors()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	return opts.Globals.PassItOn("acctExport", opts.ToCmdLine())
	// EXISTING_CODE
}

func ServeMonitors(w http.ResponseWriter, r *http.Request) bool {
	opts := FromRequest(w, r)

	err := opts.ValidateMonitors()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	if !opts.Globals.TestMode { // out test harness does not use DELETE
		delOptions := "--delete, --undelete, or --remove"
		if r.Method == "DELETE" {
			if !opts.Delete && !opts.Undelete && !opts.Remove {
				err = validate.Usage("Specify one of {0} when using the DELETE route.", delOptions)
			}
		} else {
			if opts.Delete || opts.Undelete || opts.Remove {
				delOptions = strings.Replace(delOptions, " or ", " and ", -1)
				err = validate.Usage("The {0} options are not valid when using the GET route.", delOptions)
			}
		}
		if err != nil {
			opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
			return true
		}
	}
	// opts.Globals.PassItOn("acctExport --appearances", opts.ToCmdLine())
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
