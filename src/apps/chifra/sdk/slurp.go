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

	slurp "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/slurp"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Slurp does chifra slurp
func Slurp(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	slurp.ResetOptions(false)
	opts := slurp.SlurpFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("slurp", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("slurp", w, &opts.Globals)
	err := opts.SlurpInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("slurp", err, &opts.Globals)

	return err
}

