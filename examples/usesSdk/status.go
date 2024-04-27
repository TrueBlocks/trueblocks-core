package main

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoStatus tests the When sdk function
func DoStatus() {
	logger.Info("DoStatus")

	// opts := sdk.StatusOptions{
	// 	Modes: sdk.SMBlocks,
	// }

	// if status, _, err := opts.Status(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveAndClean[map[string]interface{}]("usesSDK/status.json", status, &opts, func() error {
	// 		_, _, err := opts.Status()
	// 		return err
	// 	}); err != nil {
	// 		logger.Error(err)
	// 	}
	// }
}
