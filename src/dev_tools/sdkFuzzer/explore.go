package main

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// DoExplore tests the explore sdk function
func DoExplore() {
	ShowHeader("DoExplore", nil)

	// opts := sdk.ExploreOptions{
	// 	Mode: sdk.CMIndex,
	// }

	// buf := bytes.Buffer{}
	// if err := opts.Explore(&buf); err != nil {
	// 	logger.Error(err)
	// }

	file.StringToAsciiFile("sdkFuzzer/explore.json", "explore is not an SDK option")
}
