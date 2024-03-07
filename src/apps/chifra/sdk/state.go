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

// State provides an interface to the command line chifra state through the SDK.
func State(w io.Writer, values url.Values) error {
	testMode := os.Getenv("TEST_MODE") == "true"
	state.ResetOptions(testMode)
	opts := state.StateFinishParseInternal(w, values)
	outputHelpers.EnableCommand("state", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("state", w, &opts.Globals)
	err := opts.StateInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("state", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE

