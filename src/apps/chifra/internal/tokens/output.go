// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package tokensPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/spf13/cobra"
)

// EXISTING_CODE

func RunTokens(cmd *cobra.Command, args []string) error {
	opts := TokensFinishParse(args)

	err := opts.ValidateTokens()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	return opts.Globals.PassItOn("getTokens", opts.ToCmdLine())
	// EXISTING_CODE
}

func ServeTokens(w http.ResponseWriter, r *http.Request) bool {
	opts := TokensFinishParseApi(w, r)

	err := opts.ValidateTokens()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	// opts.Globals.PassItOn("getTokens", opts.ToCmdLine())
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
