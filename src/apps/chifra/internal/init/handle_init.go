// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import (
	"fmt"
	"sort"
	"strings"
	"sync"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
)

// InitInternal initializes local copy of UnchainedIndex by downloading manifests and chunks
func (opts *InitOptions) HandleInit() error {
	if opts.Globals.TestMode {
		return fmt.Errorf("chifra init can not be tested in test mode")
	}

	// Make the code below cleaner...
	chain := opts.Globals.Chain

	// Make sure that the temporary scraper folders are empty, so that, when the
	// scraper starts, it starts on the correct block.
	index.CleanTemporaryFolders(config.GetPathToIndex(chain), true)

	remoteManifest, err := manifest.ReadManifest(chain, manifest.FromContract)
	if err != nil {
		return err
	}
	err = remoteManifest.SaveManifest(chain)
	if err != nil {
		return err
	}

	// We sort so the download happens from latest block to earliest
	sort.Slice(remoteManifest.Chunks, func(i, j int) bool {
		return remoteManifest.Chunks[i].Range > remoteManifest.Chunks[j].Range
	})

	// Tell the user what we're doing
	logger.Log(logger.InfoC, "Unchained Index:", unchained.Address_V2)
	logger.Log(logger.InfoC, "Schemas:", unchained.Schemas)
	logger.Log(logger.InfoC, "Databases:", unchained.Databases)
	logger.Log(logger.InfoC, "Config Folder:", config.GetPathToChainConfig(chain))
	logger.Log(logger.InfoC, "Index Folder:", config.GetPathToIndex(chain))
	logger.Log(logger.InfoC, "Number of Chunks:", fmt.Sprintf("%d", len(remoteManifest.Chunks)))

	// Open a channel to receive a message when all the blooms have been downloaded...
	bloomsDoneChannel := make(chan bool)
	defer close(bloomsDoneChannel)

	// Open a channel to receive a message when all the indexes have been downloaded (if we're downloading them)
	indexDoneChannel := make(chan bool)
	defer close(indexDoneChannel)

	getChunks := func(chunkType cache.CacheType) {
		failedChunks, cancelled := downloadAndReportProgress(chain, opts.Sleep, remoteManifest.Chunks, chunkType)
		if cancelled {
			// The user hit the control+c, we don't want to continue...
			return
		}

		// The download finished...
		if len(failedChunks) > 0 {
			// ...if there were failed downloads, try them again (3 times if necessary)...
			retry(failedChunks, 3, func(items []manifest.ChunkRecord) ([]manifest.ChunkRecord, bool) {
				logger.Log(logger.Info, "Retrying", len(items), "bloom(s)")
				return downloadAndReportProgress(chain, opts.Sleep, items, chunkType)
			})
		}
	}

	// Set up a go routine to download the bloom filters...
	go func() {
		getChunks(cache.Index_Bloom)
		bloomsDoneChannel <- true
	}()

	if opts.All {
		// Set up another go routine to download the index chunks if the user told us to...
		go func() {
			getChunks(cache.Index_Final)
			indexDoneChannel <- true
		}()

		// Wait for the index to download. This will block until getChunks for index chunks returns
		<-indexDoneChannel
	}

	// Wait for the bloom filters to download. This will block until getChunks for blooms returns
	<-bloomsDoneChannel

	return nil
}

var m sync.Mutex
var nTotal int
var nProcessed int
var nStarted int
var nUpdated int

// downloadAndReportProgress Downloads the chunks and reports progress to the progressChannel
func downloadAndReportProgress(chain string, sleep float64, chunks []manifest.ChunkRecord, chunkType cache.CacheType) ([]manifest.ChunkRecord, bool) {
	failed := []manifest.ChunkRecord{}
	cancelled := false

	// Establish a channel to listen for progress messages
	progressChannel := progress.MakeChan()
	defer close(progressChannel)

	// Start the go routine that downloads the chunks. This sends messages through the progressChannel
	go index.DownloadChunks(chain, chunks, chunkType, progressChannel)

	for event := range progressChannel {
		chunk, ok := event.Payload.(*manifest.ChunkRecord)
		var rng string
		if ok {
			rng = chunk.Range
		}

		if event.Event == progress.Cancelled {
			cancelled = true
			break
		}

		if event.Event == progress.AllDone {
			msg := fmt.Sprintf("Completed updating %s files.", chunkType)
			logger.Log(logger.Info, msg, strings.Repeat(" ", 60))
			break
		}

		m.Lock()
		switch event.Event {
		case progress.Error:
			logger.Log(logger.Error, event.Message)
			if ok {
				failed = append(failed, *chunk)
			}

		case progress.Start:
			nStarted++
			logger.Log(logger.Progress, "Started download ", nStarted, " of ", nTotal, " ", event.Message, " to ", rng, spaces)

		case progress.Statistics:
			n, _ := event.Payload.(int)
			nTotal += n

		case progress.Finished:
			nProcessed++
			col := colors.Yellow
			if event.Message == "bloom" {
				col = colors.Magenta
			}
			msg := fmt.Sprintf("Unchained %s%s%s file for range %s%s%s (% 4d of %4d)", col, event.Message, colors.Off, colors.BrightBlue, rng, colors.Off, nProcessed, nTotal)
			logger.Log(logger.Info, msg, spaces)

		case progress.Update:
			msg := fmt.Sprintf("%s%s%s", colors.Yellow, event.Message, colors.Off)
			logger.Log(logger.Info, msg, spaces)
			nUpdated++

		default:
			logger.Log(logger.Info, event.Message, rng, spaces)
		}
		m.Unlock()

		// if sleep != 0.0 {
		// 	logger.Log(logger.Info, "")
		// 	logger.Log(logger.Info, "Sleeping between downloads for", sleep, "seconds")
		// 	time.Sleep(time.Duration(sleep*1000) * time.Millisecond)
		// }
	}

	return failed, cancelled
}

// retry retries downloading any `failedChunks`. It repeats `nTimes` times by calling `downloadChunks` function.
//
// Returns number of chunks that we were unable to fetch. This function is simple because:
//  1. it will never get a new failing chunk (it only feeds in the list of known, failed chunks)
//  2. The maximum number of failing chunks we can get equals the length of `failedChunks`.
func retry(failedChunks []manifest.ChunkRecord, nTimes int, downloadChunksFunc func(chunks []manifest.ChunkRecord) (failed []manifest.ChunkRecord, cancelled bool)) int {
	count := 0

	chunksToRetry := failedChunks
	cancelled := false

	for {
		if len(chunksToRetry) == 0 {
			break
		}

		if count >= nTimes {
			break
		}

		logger.Log(logger.Warning, colors.Yellow, "Retrying", len(chunksToRetry), "downloads", colors.Off)
		if chunksToRetry, cancelled = downloadChunksFunc(chunksToRetry); cancelled {
			break
		}

		count++
	}

	return len(chunksToRetry)
}

var spaces = strings.Repeat(" ", 55)
