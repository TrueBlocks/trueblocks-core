// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package quotesPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunQuotes handles the quotes command for the command line. Returns error only as per cobra.
func RunQuotes(cmd *cobra.Command, args []string) (err error) {
	opts := QuotesFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.QuotesInternal()
	return
}

// ServeQuotes handles the quotes command for the API. Returns error and a bool if handled
func ServeQuotes(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := QuotesFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	return opts.QuotesInternal()
}

// QuotesInternal handles the internal workings of the quotes command.  Returns error and a bool if handled
func (opts *QuotesOptions) QuotesInternal() (err error, handled bool) {
	err = opts.ValidateQuotes()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.Globals.ApiMode {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("getQuotes", opts.Globals.Chain, opts.ToCmdLine(), opts.GetEnvStr())
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
