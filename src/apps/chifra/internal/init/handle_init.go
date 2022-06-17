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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
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

	opts.PrintManifestHeader()

	downloadedManifest, err := pinlib.DownloadRemoteManifest(chain)
	err = opts.SaveManifest(chain, "txt", downloadedManifest)
	if err != nil {
		return err
	}
	err = opts.SaveManifest(chain, "json", downloadedManifest)
	if err != nil {
		return err
	}
	logger.Log(logger.Info, "Freshened manifest")

	// Fetch chunks
	bloomsDoneChannel := make(chan bool)
	defer close(bloomsDoneChannel)
	indexDoneChannel := make(chan bool)
	defer close(indexDoneChannel)

	getChunks := func(chunkType cache.CacheType) {
		chunkPath := cache.NewCachePath(chain, chunkType)
		failedChunks, cancelled := downloadAndReportProgress(chain, downloadedManifest.Chunks, &chunkPath)

		if cancelled {
			// We don't want to retry if the user has cancelled
			return
		}

		if len(failedChunks) > 0 {
			retry(failedChunks, 3, func(pins []manifest.Chunk) ([]manifest.Chunk, bool) {
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

type downloadFunc func(pins []manifest.Chunk) (failed []manifest.Chunk, cancelled bool)

// Downloads chunks and report progress
func downloadAndReportProgress(chain string, pins []manifest.Chunk, chunkPath *cache.CachePath) ([]manifest.Chunk, bool) {
	chunkTypeToDescription := map[cache.CacheType]string{
		cache.Index_Bloom: "bloom",
		cache.Index_Final: "index",
	}
	failed := []manifest.Chunk{}
	cancelled := false
	progressChannel := progress.MakeChan()
	defer close(progressChannel)

	go index.GetChunksFromRemote(chain, pins, chunkPath, progressChannel)

	var pinsDone uint

	for event := range progressChannel {
		pin, ok := event.Payload.(*manifest.Chunk)
		var fileName string
		if ok {
			fileName = pin.FileName
		}

		if event.Event == progress.AllDone {
			// TODO: BOGUS - can we distinguish between blooms and chunks?
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
func retry(failedPins []manifest.Chunk, times uint, downloadChunks downloadFunc) int {
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

func (opts *InitOptions) PrintManifestHeader() {
	chain := opts.Globals.Chain
	logger.Log(logger.Info, "schemas:", unchained.Schemas)
	logger.Log(logger.Info, "databases:", unchained.Databases)
	logger.Log(logger.Info, "unchainedAddress:", unchained.Address)
	logger.Log(logger.Info, "unchainedReadHash:", unchained.ReadHash)
	logger.Log(logger.Info, "unchainedPublishHash:", unchained.PublishHash)
	if !opts.Globals.TestMode {
		logger.Log(logger.Info, "manifestLocation:", config.GetPathToChainConfig(chain)) // order matters
		logger.Log(logger.Info, "unchainedIndexFolder:", config.GetPathToIndex(chain))   // order matters
	}
}

func (opts *InitOptions) SaveManifest(chain, fileType string, man *manifest.Manifest) error {
	fileName := config.GetPathToChainConfig(chain) + "manifest." + fileType
	w, err := os.Create(fileName)
	if err != nil {
		return fmt.Errorf("creating file: %s", err)
	}
	defer w.Close()
	err = file.Lock(w)
	if err != nil {
		return fmt.Errorf("locking file: %s", err)
	}

	return opts.Globals.RenderManifest(w, fileType, man)
}
