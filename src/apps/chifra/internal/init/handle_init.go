// TODO: BOGUS - IF USER DOES SCAPE FROM SCRATCH, THEN INIT, THE TIMESTAMP FILE IS INCOMPLETE

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
)

// InitInternal initializes local copy of UnchainedIndex by downloading manifests and chunks
func (opts *InitOptions) HandleInit() error {
	if opts.Globals.TestMode {
		if opts.Globals.ApiMode {
			opts.Globals.Writer.Write([]byte("{ \"msg\": \"chifra init is not processed in test mode.\" }"))
		} else {
			logger.Log(logger.Info, "chifra init is not processed in test mode.")
		}
		return nil
	}

	chain := opts.Globals.Chain

	config.EstablishIndexPaths(config.GetPathToIndex(chain))
	downloadedManifest, err := manifest.ReadManifest(chain, manifest.FromContract)
	if err != nil {
		return err
	}

	printHeader(chain, opts.Globals.TestMode)

	err = opts.SaveManifest(chain, downloadedManifest)
	if err != nil {
		return err
	}
	logger.Log(logger.Info, "Freshened manifest")

	// Fetch chunks
	bloomsDoneChannel := make(chan bool)
	defer close(bloomsDoneChannel)
	indexDoneChannel := make(chan bool)
	defer close(indexDoneChannel)
	defer func() {
		// We want to make sure these folders are empty so the scraper starts at the proper place.
		config.CleanIndexFolder(config.GetPathToIndex(chain), true)
	}()

	getChunks := func(chunkType cache.CacheType) {
		chunkPath := cache.NewCachePath(chain, chunkType)
		failedChunks, cancelled := downloadAndReportProgress(chain, downloadedManifest.Chunks, &chunkPath)

		if cancelled {
			// We don't want to retry if the user has cancelled
			return
		}

		if len(failedChunks) > 0 {
			retry(failedChunks, 3, func(pins []manifest.ChunkRecord) ([]manifest.ChunkRecord, bool) {
				logger.Log(logger.Info, "Retrying", len(pins), "bloom(s)")
				return downloadAndReportProgress(chain, pins, &chunkPath)
			})
		}
	}

	go func() {
		getChunks(cache.Index_Bloom)

		bloomsDoneChannel <- true
	}()

	if opts.All {
		go func() {
			getChunks(cache.Index_Final)

			indexDoneChannel <- true
		}()
		<-indexDoneChannel
	}

	<-bloomsDoneChannel
	return nil
}

type downloadFunc func(pins []manifest.ChunkRecord) (failed []manifest.ChunkRecord, cancelled bool)

// Downloads chunks and report progress
func downloadAndReportProgress(chain string, pins []manifest.ChunkRecord, chunkPath *cache.CachePath) ([]manifest.ChunkRecord, bool) {
	chunkTypeToDescription := map[cache.CacheType]string{
		cache.Index_Bloom: "bloom",
		cache.Index_Final: "index",
	}
	failed := []manifest.ChunkRecord{}
	cancelled := false
	progressChannel := progress.MakeChan()
	defer close(progressChannel)

	go index.DownloadChunks(chain, pins, chunkPath, progressChannel)

	var pinsDone uint

	for event := range progressChannel {
		pin, ok := event.Payload.(*manifest.ChunkRecord)
		var fileName string
		if ok {
			fileName = pin.Range
		}

		if event.Event == progress.AllDone {
			// TODO: BOGUS - FEATURE DISTINGUISH BLOOMS FROM CHUNKS IN DOWNLOAD
			logger.Log(logger.Info, pinsDone, "pin(s) were (re)initialized")
			break
		}

		if event.Event == progress.Cancelled {
			cancelled = true
			break
		}

		switch event.Event {
		case progress.Error:
			logger.Log(logger.Error, event.Message)
			if ok {
				failed = append(failed, *pin)
			}
		case progress.Start:
			logger.Log(logger.Info, "Unchaining", chunkTypeToDescription[chunkPath.Type], event.Message, "to", fileName)
		case progress.Update:
			logger.Log(logger.Info, event.Message, fileName)
		case progress.Done:
			pinsDone++
		default:
			logger.Log(logger.Info, event.Message, fileName)
		}
	}

	return failed, cancelled
}

// Retries downloading `failedPins` for `times` times by calling `downloadChunks` function.
// Returns number of pins that we were unable to fetch.
// This function is simple because: 1. it will never get a new failing pin (it only feeds in
// the list of known, failed pins); 2. The maximum number of failing pins we can get equals
// the length of `failedPins`.
func retry(failedPins []manifest.ChunkRecord, times uint, downloadChunks downloadFunc) int {
	retryCount := uint(0)

	pinsToRetry := failedPins
	cancelled := false

	for {
		if len(pinsToRetry) == 0 {
			break
		}

		if retryCount >= times {
			break
		}

		pinsToRetry, cancelled = downloadChunks(pinsToRetry)
		if cancelled {
			break
		}

		retryCount++
	}

	return len(pinsToRetry)
}

func (opts *InitOptions) SaveManifest(chain string, man *manifest.Manifest) error {
	fileName := config.GetPathToChainConfig(chain) + "manifest.json"
	w, err := os.Create(fileName)
	if err != nil {
		return fmt.Errorf("creating file: %s", err)
	}
	defer w.Close()
	err = file.Lock(w)
	if err != nil {
		return fmt.Errorf("locking file: %s", err)
	}

	tmp := opts.Globals
	tmp.Format = "json"
	tmp.Writer = w
	tmp.NoHeader = false
	tmp.ApiMode = false
	return tmp.RenderObject(man, true)
}

func printHeader(chain string, testMode bool) {
	logger.Log(logger.Info, "schemas:", unchained.Schemas)
	logger.Log(logger.Info, "databases:", unchained.Databases)
	logger.Log(logger.Info, "unchainedAddress:", unchained.Address_V2)
	logger.Log(logger.Info, "unchainedReadHash:", unchained.ReadHash_V2)
	logger.Log(logger.Info, "unchainedPublishHash:", unchained.PublishHash_V2)
	if !testMode {
		logger.Log(logger.Info, "manifestLocation:", config.GetPathToChainConfig(chain)) // order matters
		logger.Log(logger.Info, "unchainedIndexFolder:", config.GetPathToIndex(chain))   // order matters
	}
}
