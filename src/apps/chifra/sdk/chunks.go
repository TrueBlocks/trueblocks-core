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

	chunks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/chunks"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Chunks does chifra chunks
func Chunks(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	chunks.ResetOptions(false)
	opts := chunks.ChunksFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("chunks", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("chunks", w, &opts.Globals)
	err := opts.ChunksInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("chunks", err, &opts.Globals)

	return err
}

