package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoBlocks tests the Blocks sdk function
func DoBlocks() {
	logger.Info("DoBlocks")

	opts := sdk.BlocksOptions{
		BlockIds: testBlocks,
		Globals: sdk.Globals{
			Cache: true,
		},
	}

	if blocks, _, err := opts.Blocks(); err != nil {
		logger.Fatal(err)
	} else {
		SaveToFile[types.Block[types.Transaction]]("usesSDK/blocks.json", blocks)
	}

	opts.Hashes = true
	if blocks, _, err := opts.BlocksHashes(); err != nil {
		logger.Fatal(err)
	} else {
		SaveToFile[types.Block[string]]("usesSDK/blocks-hashes.json", blocks)
	}

	opts.Hashes = false
	opts.Traces = true
	if traces, _, err := opts.BlocksTraces(); err != nil {
		logger.Fatal(err)
	} else {
		SaveToFile[types.Trace]("usesSDK/blocks-traces.json", traces)
	}

	opts.Traces = false
	opts.Uniq = true
	// if apps, _, err := opts.BlocksUniq(); err != nil {
	// 	logger.Fatal(err)
	// } else {
	// 	SaveToFile[types.Appearance]("usesSDK/blocks-uniq.json", apps)
	// }

	opts.Uniq = false
	opts.Logs = true
	if logs, _, err := opts.BlocksLogs(); err != nil {
		logger.Fatal(err)
	} else {
		SaveToFile[types.Log]("usesSDK/blocks-logs.json", logs)
	}

	opts.Logs = false
	opts.Withdrawals = true
	if withdrawals, _, err := opts.BlocksWithdrawals(); err != nil {
		logger.Fatal(err)
	} else {
		SaveToFile[types.Withdrawal]("usesSDK/blocks-withdrawals.json", withdrawals)
	}

	opts.Withdrawals = false
	opts.Count = false
	if counts, _, err := opts.BlocksCount(); err != nil {
		logger.Fatal(err)
	} else {
		SaveToFile[types.BlockCount]("usesSDK/blocks-count.json", counts)
	}
}
