package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoWhen tests the when sdk function
func DoWhen() {
	opts := sdk.WhenOptions{
		BlockIds: testBlocks,
	}
	opts.Caching(sdk.CacheOn)
	ShowHeader("DoWhen", &opts)

	if when, _, err := opts.When(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.NamedBlock]("usesSDK/when.json", when, &opts, func() error {
			_, _, err := opts.When()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if whenTimestamps, _, err := opts.WhenTimestamps(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Timestamp]("usesSDK/whenTimestamps.json", whenTimestamps, &opts, func() error {
			_, _, err := opts.WhenTimestamps()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	if whenCount, _, err := opts.WhenCount(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.TimestampCount]("usesSDK/whenCount.json", whenCount, &opts, func() error {
			_, _, err := opts.WhenCount()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}
}
