// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package statePkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunState(cmd *cobra.Command, args []string) (err error) {
	opts := StateFinishParse(args)
	err, _ = opts.StateInternal()
	return
}

func ServeState(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := StateFinishParseApi(w, r)
	return opts.StateInternal()
}

func (opts *StateOptions) StateInternal() (err error, handled bool) {
	err = opts.ValidateState()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.Globals.ApiMode {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("getState", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
