package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoBlocks tests the Blocks sdk function
func DoBlocks() {
	opts := sdk.BlocksOptions{
		BlockIds:    testBlocks,
		CacheTxs:    true,
		CacheTraces: true,
	}
	opts.Caching(sdk.CacheOn)

	if blocks, _, err := opts.Blocks(); err != nil {
		logger.Fatal(err)
	} else {
		SaveAndClean[types.Block[types.Transaction]]("usesSDK/blocks.json", blocks, &opts, func() error {
			_, _, err := opts.Blocks()
			return err
		})
	}

	opts.Hashes = true
	if blocksHashes, _, err := opts.BlocksHashes(); err != nil {
		logger.Fatal(err)
	} else {
		SaveAndClean[types.Block[string]]("usesSDK/blocksHashes.json", blocksHashes, &opts, func() error {
			_, _, err := opts.BlocksHashes()
			return err
		})
	}

	opts.Hashes = false
	opts.Traces = true
	if blockTraces, _, err := opts.BlocksTraces(); err != nil {
		logger.Fatal(err)
	} else {
		SaveAndClean[types.Trace]("usesSDK/blockTraces.json", blockTraces, &opts, func() error {
			_, _, err := opts.BlocksTraces()
			return err
		})
	}

	opts.Traces = false
	opts.Uniq = true
	// if apps, _, err := opts.BlocksUniq(); err != nil {
	// 	logger.Fatal(err)
	// } else {
	// 	SaveToFile[types.Appearance]("usesSDK/blocks-uniq.json", apps)
	// opts.Caching(sdk.Decache)
	// if _, _, err := opts.Blocks(); err != nil {
	// 	logger.Fatal(err)
	// }
	// opts.Caching(sdk.CacheOn)
	// fmt.Println("Decache worked?")
	// usage.Wait()
	// }

	opts.Uniq = false
	opts.Logs = true
	if blocksLogs, _, err := opts.BlocksLogs(); err != nil {
		logger.Fatal(err)
	} else {
		SaveAndClean[types.Log]("usesSDK/blocksLogs.json", blocksLogs, &opts, func() error {
			_, _, err := opts.BlocksLogs()
			return err
		})
	}

	opts.Logs = false
	opts.Withdrawals = true
	if blocksWithdrawals, _, err := opts.BlocksWithdrawals(); err != nil {
		logger.Fatal(err)
	} else {
		SaveAndClean[types.Withdrawal]("usesSDK/blocksWithdrawals.json", blocksWithdrawals, &opts, func() error {
			_, _, err := opts.BlocksWithdrawals()
			return err
		})
	}

	opts.Withdrawals = false
	opts.Count = false
	if blocksCounts, _, err := opts.BlocksCount(); err != nil {
		logger.Fatal(err)
	} else {
		SaveAndClean[types.BlockCount]("usesSDK/blocksCounts.json", blocksCounts, &opts, func() error {
			_, _, err := opts.BlocksCount()
			return err
		})
	}
}
