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

	when "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/when"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// When does chifra when
func When(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	when.ResetOptions(false)
	opts := when.WhenFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("when", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("when", w, &opts.Globals)
	err := opts.WhenInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("when", err, &opts.Globals)

	return err
}

