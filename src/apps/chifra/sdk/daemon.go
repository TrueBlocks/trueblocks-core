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

	daemon "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/daemon"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Daemon provides an interface to the command line chifra daemon through the SDK.
func Daemon(w io.Writer, values url.Values) error {
	daemon.ResetOptions(sdkTestMode)
	opts := daemon.DaemonFinishParseInternal(w, values)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("daemon", w, &opts.Globals)
	err := opts.DaemonInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("daemon", err, &opts.Globals)

	return err
}

// EXISTING_CODE
// EXISTING_CODE
