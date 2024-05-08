package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoConfig tests the config sdk function
func DoConfig() {
	file.EstablishFolder("sdkFuzzer-output/config")
	opts := sdk.ConfigOptions{}
	ShowHeader("DoConfig", opts)

	globs := noCache(noEther(noRaw(globals)))
	for _, g := range globs {
		opts.Globals = g
		fn := getFilename("config/paths", &opts.Globals)
		TestConfig("paths", fn, &opts)
	}
}

func TestConfig(which, fn string, opts *sdk.ConfigOptions) {
	switch which {
	case "paths":
		if paths, _, err := opts.ConfigPaths(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.CacheItem](fn, paths); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	}
}
