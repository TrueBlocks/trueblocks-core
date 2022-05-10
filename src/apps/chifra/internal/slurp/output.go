// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package slurpPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunSlurp handles the slurp command for the command line. Returns error only as per cobra.
func RunSlurp(cmd *cobra.Command, args []string) (err error) {
	opts := SlurpFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.SlurpInternal()
	return
}

// ServeSlurp handles the slurp command for the API. Returns error and a bool if handled
func ServeSlurp(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := SlurpFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	return opts.SlurpInternal()
}

// SlurpInternal handles the internal workings of the slurp command.  Returns error and a bool if handled
func (opts *SlurpOptions) SlurpInternal() (err error, handled bool) {
	err = opts.ValidateSlurp()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.Globals.ApiMode {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("ethslurp", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
