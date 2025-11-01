// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	"io"
	"net/url"

	config "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output/helpers"
)

// Config provides an interface to the command line chifra config through the SDK.
func Config(rCtx *output.RenderCtx, w io.Writer, values url.Values) error {
	config.ResetOptions(sdkTestMode)
	opts := config.ConfigFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("config", w, &opts.Globals)
	err := opts.ConfigInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("config", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
