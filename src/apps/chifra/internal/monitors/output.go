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

func RunMonitors(cmd *cobra.Command, args []string) (err error) {
	opts := MonitorsFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.MonitorsInternal()
	return
}

func ServeMonitors(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := MonitorsFinishParseApi(w, r)
	// EXISTING_CODE
	if !opts.Globals.TestMode { // our test harness does not use DELETE
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
			return err, true
		}
	}
	// EXISTING_CODE
	return opts.MonitorsInternal()
}

func (opts *MonitorsOptions) MonitorsInternal() (err error, handled bool) {
	err = opts.ValidateMonitors()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	handled = true // everything is handled even on failure

	if opts.Clean {
		err = opts.HandleClean()
		return
	}

	err = opts.HandleCrudCommands()
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
