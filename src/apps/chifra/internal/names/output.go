// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package namesPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunNames handles the names command for the command line. Returns error only as per cobra.
func RunNames(cmd *cobra.Command, args []string) (err error) {
	opts := NamesFinishParse(args)
	// JINKY
	// JINKY
	err, _ = opts.NamesInternal()
	return
}

// ServeNames handles the names command for the API. Returns error and a bool if handled
func ServeNames(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := NamesFinishParseApi(w, r)
	// JINKY
	// JINKY
	return opts.NamesInternal()
}

// NamesInternal handles the internal workings of the names command.  Returns error and a bool if handled
func (opts *NamesOptions) NamesInternal() (err error, handled bool) {
	err = opts.ValidateNames()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.Globals.ApiMode {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("ethNames", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
