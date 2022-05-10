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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunState(cmd *cobra.Command, args []string) error {
	opts := StateFinishParse(args)

	err := opts.ValidateState()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	return opts.Globals.PassItOn("getState", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	// EXISTING_CODE
}

func ServeState(w http.ResponseWriter, r *http.Request) bool {
	opts := StateFinishParseApi(w, r)

	err := opts.ValidateState()
	if err != nil {
		output.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	// return opts.Globals.PassItOn("getState", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
