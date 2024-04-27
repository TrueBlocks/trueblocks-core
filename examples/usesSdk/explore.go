package main

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoExplore tests the When sdk function
func DoExplore() {
	logger.Info("DoExplore")

	// opts := sdk.ExploreOptions{
	// 	Mode: sdk.CMIndex,
	// }

	// buf := bytes.Buffer{}
	// if err := opts.Explore(&buf); err != nil {
	// 	logger.Error(err)
	// }

	file.StringToAsciiFile("usesSDK/explore.json", "explore is not an SDK option")
}
