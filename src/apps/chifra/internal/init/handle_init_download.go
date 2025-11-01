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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/walk"
)

var m sync.Mutex

// TODO: So we can capture both the blooms and the index portions in one summary. Once we move to single stream, this can go local
var nProcessed int
var nStarted int

// downloadAndReportProgress Downloads the chunks and reports progress to the progressChannel
func (opts *InitOptions) downloadAndReportProgress(chunks []types.ChunkRecord, chunkType walk.CacheType, nTotal int) ([]types.ChunkRecord, bool) {
	chain := opts.Globals.Chain
	sleep := max(.0125, opts.Sleep)
	successCount := 0

	failed := []types.ChunkRecord{}
	cancelled := false

	// Establish a channel to listen for progress messages
	progressChannel := progress.MakeChan()
	defer close(progressChannel)

	// If we make this too big, the pinning service chokes
	poolSize := runtime.NumCPU() * 2

	// Start the go routine that downloads the chunks. This sends messages through the progressChannel
	go index.DownloadChunks(chain, chunks, chunkType, poolSize, progressChannel)

	for event := range progressChannel {
		chunk, ok := event.Payload.(*types.ChunkRecord)
		var rng string
		if ok {
			rng = chunk.Range
		}

		if event.Event == progress.Cancelled {
			cancelled = true
			break
		}

		if event.Event == progress.AllDone {
			msg := fmt.Sprintf("%sCompleted initializing %s files for %s chain.%s", colors.BrightWhite, chunkType, chain, colors.Off)
			logger.Info(msg, strings.Repeat(" ", 60))
			break
		}

		// TODO: is this a performance issue?
		m.Lock() // To conflict progress printing
		switch event.Event {
		case progress.Error:
			logger.Error(event.Error)
			if ok {
				failed = append(failed, *chunk)
				if errors.Is(event.Error, index.ErrWriteToDiscError) {
					sleep = min(4, sleep*1.2)
					successCount = 0
				}
			}

		case progress.Start:
			nStarted++
			if nStarted <= 50 { // we don't need too many of these
				logger.Info("Started download of", event.Message, "[", nStarted, " of ", nTotal, "]")
			}
			if nStarted == poolSize*3 {
				msg := fmt.Sprintf("%sPlease wait...%s", colors.BrightWhite, colors.Off)
				logger.Info(msg)
			}

		case progress.Update:
			msg := fmt.Sprintf("%s%s%s", colors.Yellow, event.Message, colors.Off)
			logger.Info(msg, spaces)

		case progress.Finished:
			nProcessed++
			col := colors.Yellow
			if event.Message == "bloom" {
				col = colors.Magenta
			}
			successCount++
			pct := float64(nProcessed) / float64(nTotal) * 100
			sleepStr := ""
			if sleep > .25 {
				sleepStr = fmt.Sprintf(" [sleep: %0.2fms]", sleep*1000)
			}
			msg := fmt.Sprintf("Finished download of %s%s%s %s%s%s (% 4d of %4d %0.1f%%%s) chain: %s", col, event.Message, colors.Off, col, rng, colors.Off, nProcessed, nTotal, pct, sleepStr, opts.Globals.Chain)
			logger.Info(msg, spaces)
			if successCount%10 == 0 {
				sleep = max(.0125, sleep/1.2)
				successCount = 0
			}

		default:
			logger.Info(event.Message, rng, spaces)
		}
		m.Unlock()

		if sleep > 0 {
			ms := time.Duration(sleep*1000) * time.Millisecond
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
func retry(failedChunks []types.ChunkRecord, nTimes int, downloadChunksFunc func(chunks []types.ChunkRecord) (failed []types.ChunkRecord, cancelled bool)) int {
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

// updateLocalManifest updates the local manifest with the one downloaded but may add existing chunks if they are later...
func (opts *InitOptions) updateLocalManifest(existing, remote *manifest.Manifest) error {
	chain := opts.Globals.Chain

	copy := *remote

	if existing != nil {
		lastExisting := ranges.RangeFromRangeString(existing.Chunks[len(existing.Chunks)-1].Range)
		lastRemote := ranges.RangeFromRangeString(remote.Chunks[len(remote.Chunks)-1].Range)
		if !lastExisting.LaterThan(lastRemote) {
			for _, ch := range existing.Chunks {
				rng := ranges.RangeFromRangeString(ch.Range)
				if rng.LaterThan(lastRemote) {
					copy.Chunks = append(copy.Chunks, ch)
				}
			}
		}
	}

	return copy.SaveManifest(chain, config.PathToManifestFile(chain))
}

var spaces = strings.Repeat(" ", 55)
