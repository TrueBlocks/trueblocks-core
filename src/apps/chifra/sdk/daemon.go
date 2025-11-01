// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package sdk

import (
	"io"
	"net/url"

	daemon "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/daemon"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output/helpers"
)

// Daemon provides an interface to the command line chifra daemon through the SDK.
func Daemon(rCtx *output.RenderCtx, w io.Writer, values url.Values) error {
	daemon.ResetOptions(sdkTestMode)
	opts := daemon.DaemonFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("daemon", w, &opts.Globals)
	err := opts.DaemonInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("daemon", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
