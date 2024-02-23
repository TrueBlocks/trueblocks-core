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

	traces "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/traces"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Traces does chifra traces
func Traces(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	traces.ResetOptions(false)
	opts := traces.TracesFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("traces", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("traces", w, &opts.Globals)
	err := opts.TracesInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("traces", err, &opts.Globals)

	return err
}

