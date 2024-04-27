package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoTraces tests the When sdk function
func DoTraces() {
	logger.Info("DoTraces")

	opts := sdk.TracesOptions{
		TransactionIds: []string{"10001002.0"},
	}

	if traces, _, err := opts.Traces(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Trace]("usesSDK/traces.json", traces, &opts, func() error {
			_, _, err := opts.Traces()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if tracesCount, _, err := opts.TracesCount(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.TraceCount]("usesSDK/tracesCount.json", tracesCount, &opts, func() error {
			_, _, err := opts.TracesCount()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}
}
