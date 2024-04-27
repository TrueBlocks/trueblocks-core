package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoLogs tests the When sdk function
func DoLogs() {
	logger.Info("DoLogs")

	opts := sdk.LogsOptions{
		TransactionIds: []string{"10001002.0"},
	}

	if logs, _, err := opts.Logs(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Log]("usesSDK/logs.json", logs, &opts, func() error {
			_, _, err := opts.Logs()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}
}
