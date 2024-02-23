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

	export "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/export"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Export does chifra export
func Export(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	export.ResetOptions(false)
	opts := export.ExportFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("export", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("export", w, &opts.Globals)
	err := opts.ExportInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("export", err, &opts.Globals)

	return err
}

