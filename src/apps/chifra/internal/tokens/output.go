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

	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunTokens handles the tokens command for the command line. Returns error only as per cobra.
func RunTokens(cmd *cobra.Command, args []string) (err error) {
	opts := tokensFinishParse(args)
	outputHelpers.SetWriterForCommand("tokens", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.TokensInternal()
	return
}

// ServeTokens handles the tokens command for the API. Returns error and a bool if handled
func ServeTokens(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := tokensFinishParseApi(w, r)
	outputHelpers.InitJsonWriterApi("tokens", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.TokensInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("tokens", err, &opts.Globals)
	return
}

// TokensInternal handles the internal workings of the tokens command.  Returns error and a bool if handled
func (opts *TokensOptions) TokensInternal() (err error, handled bool) {
	err = opts.validateTokens()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.Globals.IsApiMode() {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("getTokens", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStr())
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
