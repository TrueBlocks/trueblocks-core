package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoNames tests the names sdk function
func DoNames() {
	logger.Info("DoNames")

	opts := sdk.NamesOptions{
		Terms:     []string{"Giveth"},
		MatchCase: true,
	}

	if names, _, err := opts.Names(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Name]("usesSDK/names.json", names); err != nil {
			logger.Error(err)
		}
	}

	if addrs, _, err := opts.NamesAddr(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Name]("usesSDK/names-addr.json", addrs); err != nil {
			logger.Error(err)
		}
	}

	if tags, _, err := opts.NamesTags(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Name]("usesSDK/names-tags.json", tags); err != nil {
			logger.Error(err)
		}
	}

	addr := base.HexToAddress(testAddrs[0])
	if autonames, _, err := opts.NamesAutoname(addr); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile[types.Message]("usesSDK/namesAutonames.json", autonames); err != nil {
			logger.Error(err)
		}
	}
}
