// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package abisPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunAbis(cmd *cobra.Command, args []string) error {
	opts := AbisFinishParse(args)

	err := opts.ValidateAbis()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	if len(opts.Find) > 0 {
		return opts.HandleAbiFind()
	}

	return opts.Globals.PassItOn("grabABI", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	// EXISTING_CODE
}

func ServeAbis(w http.ResponseWriter, r *http.Request) bool {
	opts := AbisFinishParseApi(w, r)

	err := opts.ValidateAbis()
	if err != nil {
		output.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	if len(opts.Find) > 0 {
		err = opts.HandleAbiFind()
		if err != nil {
			output.RespondWithError(w, http.StatusInternalServerError, err)
			return true
		}
		return true
	}

	// return opts.Globals.PassItOn("grabABI", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
