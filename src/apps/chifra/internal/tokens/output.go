// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package tokensPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunTokens handles the tokens command for the command line. Returns error only as per cobra.
func RunTokens(cmd *cobra.Command, args []string) error {
	_ = cmd
	opts := tokensFinishParse(args)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("tokens", &opts.Globals)
	return opts.TokensInternal(rCtx)
}

// ServeTokens handles the tokens command for the API. Returns an error.
func ServeTokens(w http.ResponseWriter, r *http.Request) error {
	opts := tokensFinishParseApi(w, r)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("tokens", w, &opts.Globals)
	err := opts.TokensInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("tokens", err, &opts.Globals)
	return err
}

// TokensInternal handles the internal workings of the tokens command. Returns an error.
func (opts *TokensOptions) TokensInternal(rCtx *output.RenderCtx) error {
	var err error
	if err = opts.validateTokens(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra tokens"
	// EXISTING_CODE
	// EXISTING_CODE
	if opts.Globals.Decache {
		err = opts.HandleDecache(rCtx)
	} else if len(opts.Parts) > 0 {
		err = opts.HandleParts(rCtx)
	} else {
		err = opts.HandleShow(rCtx)
	}
	timer.Report(msg)

	return err
}

// GetTokensOptions returns the options for this tool so other tools may use it.
func GetTokensOptions(args []string, g *globals.GlobalOptions) *TokensOptions {
	ret := tokensFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
