package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoExport tests the export sdk function
func DoExport() {
	opts := sdk.ExportOptions{
		Addrs: testAddrs,
		// Articulate: true,
	}
	ShowHeader("DoExport", &opts)

	if export, _, err := opts.Export(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Transaction]("sdkFuzzer/export.json", export, &opts, func() error {
			_, _, err := opts.Export()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if appearances, _, err := opts.ExportAppearances(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Appearance]("sdkFuzzer/exportAppearances.json", appearances, &opts, func() error {
			_, _, err := opts.ExportAppearances()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if receipts, _, err := opts.ExportReceipts(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Receipt]("sdkFuzzer/exportReceipts.json", receipts, &opts, func() error {
			_, _, err := opts.ExportReceipts()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if logs, _, err := opts.ExportLogs(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Log]("sdkFuzzer/exportLogs.json", logs, &opts, func() error {
			_, _, err := opts.ExportLogs()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if traces, _, err := opts.ExportTraces(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Trace]("sdkFuzzer/exportTraces.json", traces, &opts, func() error {
			_, _, err := opts.ExportTraces()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if statements, _, err := opts.ExportStatements(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Statement]("sdkFuzzer/exportStatements.json", statements, &opts, func() error {
			_, _, err := opts.ExportStatements()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if balances, _, err := opts.ExportBalances(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.State]("sdkFuzzer/exportBalances.json", balances, &opts, func() error {
			_, _, err := opts.ExportBalances()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	// if neighbors, _, err := opts.ExportNeighbors(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveAndClean[bool]("sdkFuzzer/exportNeighbors.json", neighbors, &opts, func() error {
	// 		_, _, err := opts.ExportNeighbors()
	// 		return err
	// 	}); err != nil {
	// 		logger.Error(err)
	// 	}
	// }

	if withdrawls, _, err := opts.ExportWithdrawals(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Withdrawal]("sdkFuzzer/exportWithdrawals.json", withdrawls, &opts, func() error {
			_, _, err := opts.ExportWithdrawals()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if counts, _, err := opts.ExportCount(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.AppearanceCount]("sdkFuzzer/exportCount.json", counts, &opts, func() error {
			_, _, err := opts.ExportCount()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}
}
