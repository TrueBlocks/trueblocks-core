// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

func (opts *ChunksOptions) HandleChunksExtractPins() error {
	results, err := manifest.GetPinList(opts.Globals.Chain)
	if err != nil {
		return err
	}

	if opts.Globals.TestMode {
		// Shorten the array for testing
		results = results[:100]
	}

	for i, r := range results {
		err := opts.Globals.RenderObject(r, false, i == 0)
		if err != nil {
			return err
		}
	}
	return nil
}

func (opts *ChunksOptions) PrintManifestHeader() {
	// The following two values should be read the manifest, however right now only
	// TSV format is available for download and it lacks this information
	// TODO: These values should be in a config file
	// TODO: We can add the "loaded" configuration file to Options
	// TODO: This needs to be per chain data
	chain := opts.Globals.Chain
	logger.Log(logger.Info, "hashToIndexFormatFile:", "Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2")
	logger.Log(logger.Info, "hashToBloomFormatFile:", "QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD")
	logger.Log(logger.Info, "manifestHashEncoding:", config.ReadBlockScrape(chain).UnchainedIndex.ManifestHashEncoding)
	logger.Log(logger.Info, "unchainedIndexAddr:", config.ReadBlockScrape(chain).UnchainedIndex.Address)
	if !opts.Globals.TestMode {
		logger.Log(logger.Info, "manifestLocation:", config.GetPathToChainConfig(chain)) // order matters
		logger.Log(logger.Info, "unchainedIndexFolder:", config.GetPathToIndex(chain))   // order matters
	}
}
