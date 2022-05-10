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

func RunTokens(cmd *cobra.Command, args []string) (err error) {
	opts := TokensFinishParse(args)
	err, _ = opts.TokensInternal()
	return
}

func ServeTokens(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := TokensFinishParseApi(w, r)
	return opts.TokensInternal()
}

func (opts *TokensOptions) TokensInternal() (err error, handled bool) {
	err = opts.ValidateTokens()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.Globals.ApiMode {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("getTokens", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
