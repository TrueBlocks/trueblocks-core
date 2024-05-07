package main

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// DoDaemon tests the daemon sdk function
func DoDaemon() {
	ShowHeader("DoDaemon", nil)

	// opts := sdk.DaemonOptions{
	// 	Mode: sdk.CMIndex,
	// }

	// buf := bytes.Buffer{}
	// if err := opts.Daemon(&buf); err != nil {
	// 	logger.Error(err)
	// }

	file.StringToAsciiFile("sdkFuzzer/daemon.json", "daemon is not an sdk option")
}
