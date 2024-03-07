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

	monitors "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/monitors"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Monitors provides an interface to the command line chifra monitors through the SDK.
func Monitors(w io.Writer, values url.Values) error {
	testMode := os.Getenv("TEST_MODE") == "true"
	monitors.ResetOptions(testMode)
	opts := monitors.MonitorsFinishParseInternal(w, values)
	outputHelpers.EnableCommand("monitors", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("monitors", w, &opts.Globals)
	err := opts.MonitorsInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("monitors", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE

