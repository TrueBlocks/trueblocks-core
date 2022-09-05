// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import (
	"fmt"
	"runtime"
	"sort"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
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
		failedChunks, cancelled := opts.downloadAndReportProgress(remoteManifest.Chunks, chunkType)
		if cancelled {
			// The user hit the control+c, we don't want to continue...
			return
		}

		// The download finished...
		if len(failedChunks) > 0 {
			// ...if there were failed downloads, try them again (3 times if necessary)...
			retry(failedChunks, 3, func(items []manifest.ChunkRecord) ([]manifest.ChunkRecord, bool) {
				logger.Log(logger.Info, "Retrying", len(items), "bloom(s)")
				return opts.downloadAndReportProgress(items, chunkType)
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
func (opts *InitOptions) downloadAndReportProgress(chunks []manifest.ChunkRecord, chunkType cache.CacheType) ([]manifest.ChunkRecord, bool) {
	chain := opts.Globals.Chain

	failed := []manifest.ChunkRecord{}
	cancelled := false

	// Establish a channel to listen for progress messages
	progressChannel := progress.MakeChan()
	defer close(progressChannel)

	// TODO: This should be configurable - If we make this too big, the pinning service chokes
	poolSize := utils.Min(10, (runtime.NumCPU()*3)/2)

	// Start the go routine that downloads the chunks. This sends messages through the progressChannel
	go index.DownloadChunks(chain, chunks, chunkType, poolSize, progressChannel)

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
			msg := fmt.Sprintf("Completed downloading %s files.", chunkType)
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
			if nProcessed < 20 { // we don't need too many of these
				logger.Log(logger.Progress, "Started download ", nStarted, " of ", nTotal, " ", event.Message, " to ", rng, spaces)
			}

		case progress.Update:
			msg := fmt.Sprintf("%s%s%s", colors.Yellow, event.Message, colors.Off)
			logger.Log(logger.Info, msg, spaces)
			nUpdated++

		case progress.Finished:
			nProcessed++
			col := colors.Yellow
			if event.Message == "bloom" {
				col = colors.Magenta
			}
			msg := fmt.Sprintf("Unchained %s%s%s file for range %s%s%s (% 4d of %4d)", col, event.Message, colors.Off, col, rng, colors.Off, nProcessed, nTotal)
			logger.Log(logger.Info, msg, spaces)

		case progress.Statistics:
			n, _ := event.Payload.(int)
			nTotal += n

		default:
			logger.Log(logger.Info, event.Message, rng, spaces)
		}
		m.Unlock()

		// if opts.Sleep != 0.0 {
		// 	logger.Log(logger.Info, "")
		// 	logger.Log(logger.Info, "Sleeping between downloads for", opts.Sleep, "seconds")
		// 	time.Sleep(time.Duration(opts.Sleep*1000) * time.Millisecond)
		// }
	}

	return failed, cancelled
}

// retry retries downloading any `failedChunks`. It repeats `nTimes` times by calling `downloadChunks` function.
//
// Returns number of chunks that we were unable to fetch. This function is simple because:
//  1. it will never get a new failing chunk (it only feeds in the list of known, failed chunks)
//  2. The maximum number of failing chunks we can get equals the length of `failedChunks`.
//
// TODO: Instead of storing failed attempts in an array and retrying them after processing the entire list in the manifest,
// TODO: we want to re-process failed downloads on the stop. In that way, we can do progressive backoff per chunk (as opposed
// TODO: to globally). We want to back-off on single chunks instead of every chunk. The backoff routine carries an 'attempts'
// TODO: value and we wait after each failure 2^nAttempts (double the wait each time it fails). Max 10 tries or something.
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
