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

	abis "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/abis"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Abis does chifra abis
func Abis(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	abis.ResetOptions(false)
	opts := abis.AbisFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("abis", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("abis", w, &opts.Globals)
	err := opts.AbisInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("abis", err, &opts.Globals)

	return err
}

