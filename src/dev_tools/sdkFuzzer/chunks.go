package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoChunks tests the chunks sdk function
func DoChunks() {
	opts := sdk.ChunksOptions{
		FirstBlock: 1000,
		LastBlock:  2000,
	}
	ShowHeader("ChunksManifest", &opts)
	if chunksManifest, _, err := opts.ChunksManifest(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile("sdkFuzzer/chunksManifest.json", chunksManifest); err != nil {
			logger.Error(err)
		}
	}

	ShowHeader("ChunksIndex", &opts)
	if chunksIndex, _, err := opts.ChunksIndex(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile("sdkFuzzer/chunksIndex.json", chunksIndex); err != nil {
			logger.Error(err)
		}
	}

	ShowHeader("ChunksBlooms", &opts)
	if chunksBlooms, _, err := opts.ChunksBlooms(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile("sdkFuzzer/chunksBlooms.json", chunksBlooms); err != nil {
			logger.Error(err)
		}
	}

	ShowHeader("ChunksPins", &opts)
	opts.List = true
	if chunksPins, _, err := opts.ChunksPins(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile("sdkFuzzer/chunksPins.json", chunksPins); err != nil {
			logger.Error(err)
		}
	}

	ShowHeader("ChunksAddresses", &opts)
	opts.List = false
	// if chunksAddresses, _, err := opts.ChunksAddresses(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveToFile("sdkFuzzer/chunksAddresses.json", chunksAddresses); err != nil {
	// 		logger.Error(err)
	// 	}
	// }

	ShowHeader("ChunksAppearances", &opts)
	// if chunksAppearances, _, err := opts.ChunksAppearances(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveToFile("sdkFuzzer/chunksAppearances.json", chunksAppearances, &opts, func() error {
	// 		_, _, err := opts.ChunksAppearances()
	// 		return err
	// 	}); err != nil {
	// 		logger.Error(err)
	// 	}
	// }

	ShowHeader("ChunksStats", &opts)
	if chunkStats, _, err := opts.ChunksStats(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile("sdkFuzzer/chunkStats.json", chunkStats); err != nil {
			logger.Error(err)
		}
	}
}
