// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import (
	"errors"
	"fmt"
	"runtime"
	"strings"
	"sync"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// HandleInit initializes local copy of UnchainedIndex by downloading manifests and chunks
func (opts *InitOptions) HandleInit() error {
	// Make the code below cleaner...
	chain := opts.Globals.Chain

	// TODO: BOGUS - IF THE SCRAPER IS RUNNING, THIS WILL CAUSE PROBLEMS
	// Make sure that the temporary scraper folders are empty, so that, when the
	// scraper starts, it starts on the correct block.
	_ = index.CleanTempIndexFolders(chain, []string{"ripe", "unripe", "staging"})

	remoteManifest, err := manifest.ReadManifest(chain, base.HexToAddress(opts.Publisher), manifest.FromContract)
	if err != nil {
		return err
	}

	if remoteManifest.Chain != chain {
		msg := fmt.Sprintf("The chain value found in the downloaded manifest (%s) does not match the manifest on the command line (%s).", remoteManifest.Chain, chain)
		return errors.New(msg)
	}

	err = remoteManifest.SaveManifest(chain)
	if err != nil {
		return err
	}

	// Get the list of things we need to download
	chunksToDownload, nCorrections := opts.prepareDownloadList(chain, remoteManifest, []uint64{})

	// Tell the user what we're doing
	logger.InfoTable("Unchained Index:", unchained.GetUnchainedIndexAddress())
	logger.InfoTable("Schemas:", unchained.Schemas)
	logger.InfoTable("Config Folder:", config.GetPathToChainConfig(chain))
	logger.InfoTable("Index Folder:", config.GetPathToIndex(chain))
	logger.InfoTable("Chunks in Manifest:", fmt.Sprintf("%d", len(remoteManifest.Chunks)))
	logger.InfoTable("Corrections Needed:", fmt.Sprintf("%d", nCorrections))

	// Open a channel to receive a message when all the blooms have been downloaded...
	bloomsDoneChannel := make(chan bool)
	defer close(bloomsDoneChannel)

	// Open a channel to receive a message when all the indexes have been downloaded (if we're downloading them)
	indexDoneChannel := make(chan bool)
	defer close(indexDoneChannel)

	getChunks := func(chunkType walk.CacheType) {
		failedChunks, cancelled := opts.downloadAndReportProgress(chunksToDownload, chunkType, nCorrections)
		if cancelled {
			// The user hit the control+c, we don't want to continue...
			return
		}

		// The download finished...
		if len(failedChunks) > 0 {
			// ...if there were failed downloads, try them again (3 times if necessary)...
			retry(failedChunks, 3, func(items []manifest.ChunkRecord) ([]manifest.ChunkRecord, bool) {
				logger.Info("Retrying", len(items), "bloom(s)")
				return opts.downloadAndReportProgress(items, chunkType, nCorrections)
			})
		}
	}

	// Set up a go routine to download the bloom filters...
	go func() {
		getChunks(walk.Index_Bloom)
		bloomsDoneChannel <- true
	}()

	// TODO: BOGUS - DOES THERE NEED TO BE TWO OF THESE?
	// if opts.All {
	// Set up another go routine to download the index chunks if the user told us to...
	go func() {
		getChunks(walk.Index_Final)
		indexDoneChannel <- true
	}()

	// Wait for the index to download. This will block until getChunks for index chunks returns
	<-indexDoneChannel
	// }

	// Wait for the bloom filters to download. This will block until getChunks for blooms returns
	<-bloomsDoneChannel

	return nil
}

var m sync.Mutex

// TODO: So we can capture both the blooms and the index portions in one summary. Once we move to single stream, this can go local
var nProcessed12 int
var nStarted12 int

// downloadAndReportProgress Downloads the chunks and reports progress to the progressChannel
func (opts *InitOptions) downloadAndReportProgress(chunks []manifest.ChunkRecord, chunkType walk.CacheType, nTotal int) ([]manifest.ChunkRecord, bool) {
	chain := opts.Globals.Chain

	failed := []manifest.ChunkRecord{}
	cancelled := false

	// Establish a channel to listen for progress messages
	progressChannel := progress.MakeChan()
	defer close(progressChannel)

	// TODO: BOGUS This should be configurable - If we make this too big, the pinning service chokes
	poolSize := runtime.NumCPU() * 2

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
			msg := fmt.Sprintf("%sCompleted initializing %s files.%s", colors.BrightWhite, chunkType, colors.Off)
			logger.Info(msg, strings.Repeat(" ", 60))
			break
		}

		// TODO: is this a performance issue?
		m.Lock() // To conflict progress printing
		switch event.Event {
		case progress.Error:
			logger.Error(event.Message)
			if ok {
				failed = append(failed, *chunk)
			}

		case progress.Start:
			nStarted12++
			if nProcessed12 < 20 { // we don't need too many of these
				logger.Info("Started download ", nStarted12, " of ", nTotal, " ", event.Message)
			}
			if nStarted12 == poolSize*3 {
				msg := fmt.Sprintf("%sPlease wait...%s", colors.BrightWhite, colors.Off)
				logger.Info(msg)
			}

		case progress.Update:
			msg := fmt.Sprintf("%s%s%s", colors.Yellow, event.Message, colors.Off)
			logger.Info(msg, spaces)

		case progress.Finished:
			nProcessed12++
			col := colors.Yellow
			if event.Message == "bloom" {
				col = colors.Magenta
			}
			msg := fmt.Sprintf("Unchained %s%s%s file for range %s%s%s (% 4d of %4d)", col, event.Message, colors.Off, col, rng, colors.Off, nProcessed12, nTotal)
			logger.Info(msg, spaces)

		default:
			logger.Info(event.Message, rng, spaces)
		}
		m.Unlock()

		sleep := opts.Sleep
		if sleep > 0 {
			ms := time.Duration(sleep*1000) * time.Millisecond
			if !opts.Globals.TestMode {
				logger.Info(fmt.Sprintf("Sleeping for %g seconds", sleep))
			}
			time.Sleep(ms)
		}
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

		logger.Warn(colors.Yellow, "Retrying", len(chunksToRetry), "downloads", colors.Off)
		if chunksToRetry, cancelled = downloadChunksFunc(chunksToRetry); cancelled {
			break
		}

		count++
	}

	return len(chunksToRetry)
}

var spaces = strings.Repeat(" ", 55)
