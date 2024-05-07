package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoNames tests the names sdk function
func DoNames() {
	opts := sdk.NamesOptions{
		Terms:     []string{"Giveth"},
		MatchCase: true,
	}
	ShowHeader("DoNames", &opts)

	if names, _, err := opts.Names(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Name]("sdkFuzzer/names.json", names); err != nil {
			logger.Error(err)
		}
	}

	if addrs, _, err := opts.NamesAddr(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Name]("sdkFuzzer/names-addr.json", addrs); err != nil {
			logger.Error(err)
		}
	}

	if tags, _, err := opts.NamesTags(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Name]("sdkFuzzer/names-tags.json", tags); err != nil {
			logger.Error(err)
		}
	}

	addr := base.HexToAddress(testAddrs[0])
	if autonames, _, err := opts.NamesAutoname(addr); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Message]("sdkFuzzer/namesAutonames.json", autonames); err != nil {
			logger.Error(err)
		}
	}
}
