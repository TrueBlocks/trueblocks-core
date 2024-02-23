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

// Monitors does chifra monitors
func Monitors(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	monitors.ResetOptions(false)
	opts := monitors.MonitorsFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("monitors", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("monitors", w, &opts.Globals)
	err := opts.MonitorsInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("monitors", err, &opts.Globals)

	return err
}

