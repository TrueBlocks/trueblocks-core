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

	state "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/state"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// State does chifra state
func State(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	state.ResetOptions(false)
	opts := state.StateFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("state", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("state", w, &opts.Globals)
	err := opts.StateInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("state", err, &opts.Globals)

	return err
}

