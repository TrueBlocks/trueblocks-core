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

	daemon "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/daemon"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
)

// Daemon does chifra daemon
func Daemon(w io.Writer, options map[string]string) error {
	values := make(url.Values)
	for key, val := range options {
		values.Set(key, val)
	}

	daemon.ResetOptions(false)
	opts := daemon.DaemonFinishParseInternal(os.Stdout, values)
	outputHelpers.EnableCommand("daemon", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("daemon", w, &opts.Globals)
	err := opts.DaemonInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("daemon", err, &opts.Globals)

	return err
}

