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

// RunExport handles the export command for the command line. Returns error only as per cobra.
func RunExport(cmd *cobra.Command, args []string) (err error) {
	opts := ExportFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.ExportInternal()
	return
}

// ServeExport handles the export command for the API. Returns error and a bool if handled
func ServeExport(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := ExportFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	return opts.ExportInternal()
}

// ExportInternal handles the internal workings of the export command.  Returns error and a bool if handled
func (opts *ExportOptions) ExportInternal() (err error, handled bool) {
	err = opts.ValidateExport()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	err = opts.FreshenMonitorsForExport()
	if err != nil {
		return err, true
	}

	if opts.Globals.ApiMode {
		// The caller has to handle this when in API mode
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("acctExport", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
