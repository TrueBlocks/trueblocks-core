// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package pinsPkg

import (
	"net/url"
	"path"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/chunk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
)

// InitInternal initializes local copy of UnchainedIndex by downloading manifests and chunks
func (opts *PinsOptions) InitInternal() error {

	config.EstablishIndexPaths(config.GetPathToIndex(opts.Globals.Chain))

	opts.PrintManifestHeader()

	// Fetch manifest's CID
	cid, err := pinlib.GetManifestCidFromContract(opts.Globals.Chain)
	if err != nil {
		return err
	}
	logger.Log(logger.Info, "Unchained index returned CID", cid)

	// Download the manifest
	gatewayUrl := config.ReadBlockScrape(opts.Globals.Chain).Dev.IpfsGateway
	logger.Log(logger.Info, "IPFS gateway", gatewayUrl)

	url, err := url.Parse(gatewayUrl)
	if err != nil {
		return err
	}
	url.Path = path.Join(url.Path, cid)
	downloadedManifest, err := pinlib.DownloadManifest(url.String())

	if err != nil {
		return err
	}

	// Save manifest
	manifestPath := config.GetPathToChainConfig(opts.Globals.Chain) + "manifest.txt"
	err = pinlib.SaveManifest(manifestPath, downloadedManifest)
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
		chunkPath := &cache.Path{}
		chunkPath.New(opts.Globals.Chain, chunkType)
		failedChunks, cancelled := downloadAndReportProgress(opts.Globals.Chain, downloadedManifest.NewPins, chunkPath)

		if cancelled {
			// We don't want to retry if the user has cancelled
			return
		}

		if len(failedChunks) > 0 {
			retry(failedChunks, 3, func(pins []manifest.PinDescriptor) ([]manifest.PinDescriptor, bool) {
				logger.Log(logger.Info, "Retrying", len(pins), "bloom(s)")
				return downloadAndReportProgress(opts.Globals.Chain, pins, chunkPath)
			})
		}
	}

	go func() {
		getChunks(cache.BloomChunk)

		bloomsDoneChannel <- true
	}()

	if opts.All {
		go func() {
			getChunks(cache.IndexChunk)

			indexDoneChannel <- true
		}()
		<-indexDoneChannel
	}

	<-bloomsDoneChannel
	return nil
}

type downloadFunc func(pins []manifest.PinDescriptor) (failed []manifest.PinDescriptor, cancelled bool)

// Downloads chunks and report progress
func downloadAndReportProgress(chain string, pins []manifest.PinDescriptor, chunkPath *cache.Path) ([]manifest.PinDescriptor, bool) {
	chunkTypeToDescription := map[cache.CacheType]string{
		cache.BloomChunk: "bloom",
		cache.IndexChunk: "index",
	}
	failed := []manifest.PinDescriptor{}
	cancelled := false
	progressChannel := progress.MakeChan()
	defer close(progressChannel)

	go chunk.GetChunksFromRemote(chain, pins, chunkPath, progressChannel)

	var pinsDone uint

	for event := range progressChannel {
		pin, ok := event.Payload.(*manifest.PinDescriptor)
		var fileName string
		if ok {
			fileName = pin.FileName
		}

		if event.Event == progress.AllDone {
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
func retry(failedPins []manifest.PinDescriptor, times uint, downloadChunks downloadFunc) int {
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

func (opts *PinsOptions) PrintManifestHeader() {
	// The following two values should be read the manifest, however right now only
	// TSV format is available for download and it lacks this information
	// TODO: These values should be in a config file
	// TODO: We can add the "loaded" configuration file to Options
	// TODO: This needs to be per chain data
	logger.Log(logger.Info, "hashToIndexFormatFile:", "Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2")
	logger.Log(logger.Info, "hashToBloomFormatFile:", "QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD")
	logger.Log(logger.Info, "manifestHashEncoding:", config.ReadBlockScrape(opts.Globals.Chain).UnchainedIndex.ManifestHashEncoding)
	logger.Log(logger.Info, "unchainedIndexAddr:", config.ReadBlockScrape(opts.Globals.Chain).UnchainedIndex.Address)
	if !opts.Globals.TestMode {
		logger.Log(logger.Info, "unchainedIndexFolder:", config.GetPathToIndex(opts.Globals.Chain))
		logger.Log(logger.Info, "manifestLocation:", config.GetPathToChainConfig(opts.Globals.Chain))
	}
}
