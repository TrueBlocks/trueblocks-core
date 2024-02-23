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

	config "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/config"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Config does chifra config
func Config(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	config.ResetOptions(false)
	opts := config.ConfigFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("config", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("config", w, &opts.Globals)
	err := opts.ConfigInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("config", err, &opts.Globals)

	return err
}

