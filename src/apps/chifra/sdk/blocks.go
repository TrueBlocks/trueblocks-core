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

	blocks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/blocks"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Blocks does chifra blocks
func Blocks(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	blocks.ResetOptions(false)
	opts := blocks.BlocksFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("blocks", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("blocks", w, &opts.Globals)
	err := opts.BlocksInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("blocks", err, &opts.Globals)

	return err
}

