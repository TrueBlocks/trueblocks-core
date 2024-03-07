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

	explore "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/explore"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Explore provides an interface to the command line chifra explore through the SDK.
func Explore(w io.Writer, values url.Values) error {
	explore.ResetOptions(os.Getenv("TEST_MODE") == "true")
	opts := explore.ExploreFinishParseInternal(w, values)
	outputHelpers.EnableCommand("explore", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("explore", w, &opts.Globals)
	err := opts.ExploreInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("explore", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
