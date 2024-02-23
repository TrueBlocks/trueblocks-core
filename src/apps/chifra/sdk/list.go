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

	list "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/list"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// List provides an interface to the command line chifra list through the SDK.
func List(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	list.ResetOptions(false)
	opts := list.ListFinishParseInternal(w, values)
	outputHelpers.EnableCommand("list", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("list", w, &opts.Globals)
	err := opts.ListInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("list", err, &opts.Globals)

	return err
}

