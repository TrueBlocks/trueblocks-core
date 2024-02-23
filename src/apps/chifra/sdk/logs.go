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

	logs "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/logs"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Logs does chifra logs
func Logs(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	logs.ResetOptions(false)
	opts := logs.LogsFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("logs", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("logs", w, &opts.Globals)
	err := opts.LogsInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("logs", err, &opts.Globals)

	return err
}

