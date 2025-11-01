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

	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/init"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output/helpers"
)

// Init provides an interface to the command line chifra init through the SDK.
func Init(rCtx *output.RenderCtx, w io.Writer, values url.Values) error {
	initPkg.ResetOptions(sdkTestMode)
	opts := initPkg.InitFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("init", w, &opts.Globals)
	err := opts.InitInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("init", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
