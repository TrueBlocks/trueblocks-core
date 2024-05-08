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

	// BlockIds   []string     `json:"blocks,omitempty"`
	// Check      bool         `json:"check,omitempty"`
	// Pin        bool         `json:"pin,omitempty"`
	// Publish    bool         `json:"publish,omitempty"`
	// Publisher  base.Address `json:"publisher,omitempty"`
	// Remote     bool         `json:"remote,omitempty"`
	// Belongs    []string     `json:"belongs,omitempty"`
	// FirstBlock base.Blknum  `json:"firstBlock,omitempty"`
	// LastBlock  base.Blknum  `json:"lastBlock,omitempty"`
	// MaxAddrs   uint64       `json:"maxAddrs,omitempty"`
	// Deep       bool         `json:"deep,omitempty"`
	// Rewrite    bool         `json:"rewrite,omitempty"`
	// List       bool         `json:"list,omitempty"`
	// Unpin      bool         `json:"unpin,omitempty"`
	// Count      bool         `json:"count,omitempty"`
	// Sleep      float64      `json:"sleep,omitempty"`
	// func (opts *ChunksOptions) ChunksManifest() ([]types.ChunkManifest, *types.MetaData, error) {
	// func (opts *ChunksOptions) ChunksIndex() ([]types.ChunkIndex, *types.MetaData, error) {
	// func (opts *ChunksOptions) ChunksBlooms() ([]types.ChunkBloom, *types.MetaData, error) {
	// func (opts *ChunksOptions) ChunksPins() ([]types.ChunkPin, *types.MetaData, error) {
	// func (opts *ChunksOptions) ChunksAddresses() ([]types.ChunkAddress, *types.MetaData, error) {
	// func (opts *ChunksOptions) ChunksAppearances() ([]types.ChunkAppearance, *types.MetaData, error) {
	// func (opts *ChunksOptions) ChunksStats() ([]types.ChunkStats, *types.MetaData, error) {
	// func (opts *ChunksOptions) ChunksTruncate(val base.Blknum) ([]types.Message, *types.MetaData, error) {
	// func (opts *ChunksOptions) ChunksDiff() ([]types.Message, *types.MetaData, error) {
	// func (opts *ChunksOptions) ChunksTag(val string) ([]types.Message, *types.MetaData, error) {
	// NoCHM      ChunksMode = 0
	// CMManifest            = 1 << iota
	// CMIndex
	// CMBlooms
	// CMPins
	// CMAddresses
	// CMAppearances
	// CMStats

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
