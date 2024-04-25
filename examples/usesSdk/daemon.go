package main

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoDaemon tests the When sdk function
func DoDaemon() {
	logger.Info("DoDaemon")

	// opts := sdk.DaemonOptions{
	// 	Mode: sdk.CMIndex,
	// }

	// buf := bytes.Buffer{}
	// if err := opts.Daemon(&buf); err != nil {
	// 	logger.Fatal(err)
	// }

	file.StringToAsciiFile("usesSDK/daemon.json", "daemon is not an sdk option")
}
