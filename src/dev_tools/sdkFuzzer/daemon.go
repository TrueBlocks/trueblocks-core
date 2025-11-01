// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
package main

// EXISTING_CODE
import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v6"
)

// EXISTING_CODE

// DoDaemon tests the Daemon sdk function
func DoDaemon() {
	file.EstablishFolder("sdkFuzzer-output/daemon")
	opts := sdk.DaemonOptions{}
	ShowHeader("DoDaemon", opts)

	// EXISTING_CODE
	// daemon,command,verbose|version|noop|noColor|
	// opts := sdk.DaemonOptions{
	// 	Mode: sdk.CMIndex,
	// }

	// buf := bytes.Buffer{}
	// if err := opts.Daemon(&buf); err != nil {
	// 	logger.Error(err)
	// }

	file.StringToAsciiFile("sdkFuzzer-output/daemon.json", "daemon is not an sdk option")
	// EXISTING_CODE
	Wait()
}

func TestDaemon(which, value, fn string, opts *sdk.DaemonOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	_ = fn // silence warning
	// EXISTING_CODE

	switch which {
	}
}

// EXISTING_CODE
// EXISTING_CODE
