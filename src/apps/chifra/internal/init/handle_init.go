// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/history"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// HandleInit initializes local copy of UnchainedIndex by downloading manifests and chunks
func (opts *InitOptions) HandleInit() error {
	_ = context.TODO()
	// Make the code below cleaner...
	chain := opts.Globals.Chain

	// TODO: BOGUS - IF THE SCRAPER IS RUNNING, THIS WILL CAUSE PROBLEMS
	// Make sure that the temporary scraper folders are empty, so that, when the
	// scraper starts, it starts on the correct block.
	_ = file.CleanFolder(chain, config.PathToIndex(chain), []string{"ripe", "unripe", "maps", "staging"})

	existing, err := manifest.ReadManifest(chain, opts.PublisherAddr, manifest.LocalCache)
	if err != nil {
		return err
	}

	remote, err := manifest.ReadManifest(chain, opts.PublisherAddr, manifest.FromContract)
	if err != nil {
		return err
	}

	if err = opts.updateLocalManifest(existing, remote); err != nil {
		return err
	}

	// Get the list of things we need to download
	chunksToDownload, nToDownload, nDeleted, err := opts.prepareDownloadList(chain, remote, []base.Blknum{})
	if err != nil {
		return err
	}

	// Tell the user what we're doing
	logger.InfoTable("Unchained Index:", config.GetUnchained().SmartContract)
	logger.InfoTable("PreferredPublisher:", opts.Publisher)
	logger.InfoTable("Specification:", manifest.Specification())
	logger.InfoTable("Config Folder:", config.MustGetPathToChainConfig(chain))
	logger.InfoTable("Index Folder:", config.PathToIndex(chain))
	logger.InfoTable("Chunks in manifest:", fmt.Sprintf("%d", len(remote.Chunks)))
	logger.InfoTable("Files deleted:", fmt.Sprintf("%d", nDeleted))
	logger.InfoTable("Files downloaded:", fmt.Sprintf("%d", nToDownload))

	historyFile := config.PathToCache(chain) + "tmp/history.txt"
	if opts.All && !history.FromHistoryBool(historyFile, "init") {
		_ = history.ToHistory(historyFile, "init", "true")
	}

	// Open a channel to receive a message when all the blooms have been downloaded...
	bloomsDoneChannel := make(chan bool)
	defer close(bloomsDoneChannel)

	// Open a channel to receive a message when all the indexes have been downloaded (if we're downloading them)
	indexDoneChannel := make(chan bool)
	defer close(indexDoneChannel)

	getChunks := func(chunkType walk.CacheType) {
		failedChunks, cancelled := opts.downloadAndReportProgress(chunksToDownload, chunkType, nToDownload)
		if cancelled {
			// The user hit the control+c, we don't want to continue...
			return
		}

		// The download finished...
		if len(failedChunks) > 0 {
			// ...if there were failed downloads, try them again (3 times if necessary)...
			retry(failedChunks, 3, func(items []types.ChunkRecord) ([]types.ChunkRecord, bool) {
				logger.Info("Retrying", len(items), "bloom(s)")
				return opts.downloadAndReportProgress(items, chunkType, nToDownload)
			})
		}
	}

	// Set up a go routine to download the bloom filters...
	go func() {
		getChunks(walk.Index_Bloom)
		bloomsDoneChannel <- true
	}()

	// TODO: BOGUS - WHY DOES THERE NEED TO BE TWO OF THESE?
	// Set up another go routine to download the index chunks if the user told us to...
	go func() {
		getChunks(walk.Index_Final)
		indexDoneChannel <- true
	}()

	// Wait for the index to download. This will block until getChunks for index chunks returns
	<-indexDoneChannel

	// Wait for the bloom filters to download. This will block until getChunks for blooms returns
	<-bloomsDoneChannel

	if nDeleted+nToDownload > 0 {
		logger.Warn("The on-disk index has changed. You must invalidate your monitor cache by removing it.")
	}

	return nil
}

// HandleShow initializes local copy of UnchainedIndex by downloading manifests and chunks
func (opts *InitOptions) HandleShow() error {
	return opts.HandleInit()
}
