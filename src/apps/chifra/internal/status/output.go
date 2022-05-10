// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package statusPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunStatus(cmd *cobra.Command, args []string) (err error) {
	opts := StatusFinishParse(args)
	err, _ = opts.StatusInternal()
	return
}

func ServeStatus(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := StatusFinishParseApi(w, r)
	return opts.StatusInternal()
}

func (opts *StatusOptions) StatusInternal() (err error, handled bool) {
	err = opts.ValidateStatus()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.Globals.ApiMode {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("cacheStatus", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
