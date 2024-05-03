package main

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoConfig tests the config sdk function
func DoConfig() {
	logger.Info("DoConfig")

	// opts := sdk.ConfigOptions{
	// 	Mode: sdk.CMShow,
	// }

	// if config, _, err := opts.Config(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveAndClean[types.Config]("usesSDK/config.json", config, &opts, func() error {
	// 		_, _, err := opts.Config()
	// 		return err
	// 	}); err != nil {
	// 		logger.Error(err)
	// 	}
	// }
}
