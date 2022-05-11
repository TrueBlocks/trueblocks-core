// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package initPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunInit handles the init command for the command line. Returns error only as per cobra.
func RunInit(cmd *cobra.Command, args []string) (err error) {
	opts := InitFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.InitInternal()
	return
}

// ServeInit handles the init command for the API. Returns error and a bool if handled
func ServeInit(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := InitFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	return opts.InitInternal()
}

// InitInternal handles the internal workings of the init command.  Returns error and a bool if handled
func (opts *InitOptions) InitInternal() (err error, handled bool) {
	err = opts.ValidateInit()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	handled = true

	err = opts.HandleInit()
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
