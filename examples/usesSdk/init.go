package main

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoInit tests the init sdk function
func DoInit() {
	logger.Info("DoInit")

	// opts := sdk.InitOptions{}

	// if init, _, err := opts.InitAll(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveAndClean[bool]("usesSDK/init.json", init, &opts, func() error {
	// 		_, _, err := opts.InitAll()
	// 		return err
	// 	}); err != nil {
	// 		logger.Error(err)
	// 	}
	// }
}
