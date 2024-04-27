package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoNames tests the When sdk function
func DoNames() {
	logger.Info("DoNames")

	opts := sdk.NamesOptions{
		Terms:     []string{"Giveth"},
		MatchCase: true,
	}

	if names, _, err := opts.Names(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Name]("usesSDK/names.json", names, &opts, func() error {
			_, _, err := opts.Names()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	// opts.Addr = true
	// if addrs, _, err := opts.NamesAddr(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveAndClean[base.Address]("usesSDK/names-addr.json", addrs, &opts, func() error {
	// 		_, _, err := opts.NamesAddr()
	// 		return err
	// 	})
	// }

	// opts.Addr = false
	// opts.Tags = true
	// if tags, _, err := opts.NamesTags(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveAndClean[string]("usesSDK/names-tags.json", tags, &opts, func() error {
	// 		_, _, err := opts.NamesTags()
	// 		return err
	// 	})
	// }
}
