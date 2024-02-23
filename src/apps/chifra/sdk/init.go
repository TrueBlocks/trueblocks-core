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

	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/init"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Init provides an interface to the command line chifra init through the SDK.
func Init(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	initPkg.ResetOptions(false)
	opts := initPkg.InitFinishParseInternal(w, values)
	outputHelpers.EnableCommand("init", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("init", w, &opts.Globals)
	err := opts.InitInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("init", err, &opts.Globals)

	return err
}

