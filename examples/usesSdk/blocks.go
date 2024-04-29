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
		BlockIds:    testBlocks,
		CacheTxs:    true,
		CacheTraces: true,
	}
	opts.Caching(sdk.CacheOn)

	if blocks, _, err := opts.Blocks(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Block[types.Transaction]]("usesSDK/blocks.json", blocks, &opts, func() error {
			_, _, err := opts.Blocks()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if blocksHashes, _, err := opts.BlocksHashes(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Block[string]]("usesSDK/blocksHashes.json", blocksHashes, &opts, func() error {
			_, _, err := opts.BlocksHashes()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if blockTraces, _, err := opts.BlocksTraces(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Trace]("usesSDK/blockTraces.json", blockTraces, &opts, func() error {
			_, _, err := opts.BlocksTraces()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	// if blockApps, _, err := opts.BlocksUniq(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveAndClean[types.Appearance]("usesSDK/blocks-uniq.json", blockApps, &opts, func() error {
	// 		_, _, err := opts.BlocksUniq()
	// 		return err
	// 	}); err != nil {
	// logger.Error(err)
	// }
	// }

	if blocksLogs, _, err := opts.BlocksLogs(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Log]("usesSDK/blocksLogs.json", blocksLogs, &opts, func() error {
			_, _, err := opts.BlocksLogs()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if blocksWithdrawals, _, err := opts.BlocksWithdrawals(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Withdrawal]("usesSDK/blocksWithdrawals.json", blocksWithdrawals, &opts, func() error {
			_, _, err := opts.BlocksWithdrawals()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if blocksCounts, _, err := opts.BlocksCount(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.BlockCount]("usesSDK/blocksCounts.json", blocksCounts, &opts, func() error {
			_, _, err := opts.BlocksCount()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}
}
