// Copyright 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	"io"
	"net/url"
	"os"

	tokens "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/tokens"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Tokens does chifra tokens
func Tokens(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	tokens.ResetOptions(false)
	opts := tokens.TokensFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("tokens", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("tokens", w, &opts.Globals)
	err := opts.TokensInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("tokens", err, &opts.Globals)

	return err
}

