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

	names "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/names"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Names provides an interface to the command line chifra names through the SDK.
func Names(w io.Writer, values url.Values) error {
	testMode := os.Getenv("TEST_MODE") == "true"
	names.ResetOptions(testMode)
	opts := names.NamesFinishParseInternal(w, values)
	outputHelpers.EnableCommand("names", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("names", w, &opts.Globals)
	err := opts.NamesInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("names", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE

