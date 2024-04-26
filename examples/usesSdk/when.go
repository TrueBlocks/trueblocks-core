package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoWhen tests the When sdk function
func DoWhen() {
	opts := sdk.WhenOptions{
		BlockIds: testBlocks,
	}
	opts.Caching(sdk.CacheOn)

	if when, _, err := opts.When(); err != nil {
		logger.Fatal(err)
	} else {
		SaveAndClean[types.NamedBlock]("usesSDK/when.json", when, &opts, func() error {
			_, _, err := opts.When()
			return err
		})
	}

	opts.Timestamps = true
	if whenTimestamps, _, err := opts.WhenTimestamps(); err != nil {
		logger.Fatal(err)
	} else {
		SaveAndClean[types.Timestamp]("usesSDK/whenTimestamps.json", whenTimestamps, &opts, func() error {
			_, _, err := opts.WhenTimestamps()
			return err
		})
	}

	// opts.Timestamps = false
	opts.Count = true
	if whenCount, _, err := opts.WhenCount(); err != nil {
		logger.Fatal(err)
	} else {
		SaveAndClean[types.TimestampCount]("usesSDK/whenCount.json", whenCount, &opts, func() error {
			_, _, err := opts.WhenCount()
			return err
		})
	}
}
