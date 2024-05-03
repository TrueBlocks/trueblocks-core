package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoChunks tests the chunks sdk function
func DoChunks() {
	logger.Info("DoChunks")

	opts := sdk.ChunksOptions{
		FirstBlock: 1000,
		LastBlock:  2000,
	}

	logger.Info("ChunksManifest")
	if chunksManifest, _, err := opts.ChunksManifest(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile("usesSDK/chunksManifest.json", chunksManifest); err != nil {
			logger.Error(err)
		}
	}

	logger.Info("ChunksIndex")
	if chunksIndex, _, err := opts.ChunksIndex(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile("usesSDK/chunksIndex.json", chunksIndex); err != nil {
			logger.Error(err)
		}
	}

	logger.Info("ChunksBlooms")
	if chunksBlooms, _, err := opts.ChunksBlooms(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile("usesSDK/chunksBlooms.json", chunksBlooms); err != nil {
			logger.Error(err)
		}
	}

	logger.Info("ChunksPins")
	opts.List = true
	if chunksPins, _, err := opts.ChunksPins(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile("usesSDK/chunksPins.json", chunksPins); err != nil {
			logger.Error(err)
		}
	}

	logger.Info("ChunksAddresses")
	opts.List = false
	// if chunksAddresses, _, err := opts.ChunksAddresses(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveToFile("usesSDK/chunksAddresses.json", chunksAddresses); err != nil {
	// 		logger.Error(err)
	// 	}
	// }

	logger.Info("ChunksAppearances")
	// if chunksAppearances, _, err := opts.ChunksAppearances(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveToFile("usesSDK/chunksAppearances.json", chunksAppearances, &opts, func() error {
	// 		_, _, err := opts.ChunksAppearances()
	// 		return err
	// 	}); err != nil {
	// 		logger.Error(err)
	// 	}
	// }

	logger.Info("ChunksStats")
	if chunkStats, _, err := opts.ChunksStats(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveToFile("usesSDK/chunkStats.json", chunkStats); err != nil {
			logger.Error(err)
		}
	}
}
