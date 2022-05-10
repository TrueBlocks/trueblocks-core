// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package whenPkg

// EXISTING_CODE
import (
	"net/http"
	"strings"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunWhen handles the when command for the command line. Returns error only as per cobra.
func RunWhen(cmd *cobra.Command, args []string) (err error) {
	opts := WhenFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.WhenInternal()
	return
}

// ServeWhen handles the when command for the API. Returns error and a bool if handled
func ServeWhen(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := WhenFinishParseApi(w, r)
	// EXISTING_CODE
	if opts.Globals.ApiMode {
		// weird special case since we have to alter the request
		if opts.List {
			err = opts.ValidateWhen()
			if err != nil {
				return err, true
			}
			err = opts.HandleWhenList()
			if err != nil {
				return err, true
			}
			if len(opts.Blocks) == 0 {
				return nil, true
			}
			r.URL.RawQuery = strings.Replace(r.URL.RawQuery, "list", "noop", -1)
			r.URL.RawQuery += "&no_header"
		}
	}
	// EXISTING_CODE
	return opts.WhenInternal()
}

// WhenInternal handles the internal workings of the when command.  Returns error and a bool if handled
func (opts *WhenOptions) WhenInternal() (err error, handled bool) {
	err = opts.ValidateWhen()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.List {
		err = opts.HandleWhenList()
		if err != nil {
			return err, true
		}
		if len(opts.Blocks) == 0 {
			return nil, true
		}
		// continue but don't show headers
		opts.List = false
		opts.Globals.NoHeader = true
	}

	if opts.Globals.ApiMode {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("whenBlock", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
