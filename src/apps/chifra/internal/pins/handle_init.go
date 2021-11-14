package pinsPkg

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

import (
	"net/url"
	"path"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/chunk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
)

// HandleInit calls Init() and prints error, if any
func HandleInit(opts *PinsOptionsType) {
	err := InitInternal(opts)
	if err != nil {
		logger.Fatal(err)
	}
}

// InitInternal initializes local copy of UnchainedIndex by downloading manifests and chunks
func InitInternal(opts *PinsOptionsType) error {
	logger.Log(logger.Info, "Calling unchained index smart contract...")

	// Fetch manifest's CID
	cid, err := pinlib.GetManifestCidFromContract()
	if err != nil {
		return err
	}
	logger.Log(logger.Info, "Found manifest hash at", cid)

	// Download the manifest
	gatewayUrl := config.ReadBlockScrape().Dev.IpfsGateway
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
	err = pinlib.SaveManifest(config.GetConfigPath("manifest/manifest.txt"), downloadedManifest)
	if err != nil {
		return err
	}
	logger.Log(logger.Info, "Freshened manifest")

	// Fetch chunks
	bloomsDoneChannel := make(chan bool)
	defer close(bloomsDoneChannel)
	indexDoneChannel := make(chan bool)
	defer close(indexDoneChannel)

	getChunks := func(chunkType chunk.ChunkType) {
		failedChunks := downloadAndReportProgress(downloadedManifest.NewPins, chunkType)

		if len(failedChunks) > 0 {
			retry(failedChunks, 3, func(pins []manifest.PinDescriptor) []manifest.PinDescriptor {
				logger.Log(logger.Info, "Retrying", len(pins), "bloom(s)")
				return downloadAndReportProgress(pins, chunkType)
			})
		}
	}

	go func() {
		getChunks(chunk.BloomChunk)

		bloomsDoneChannel <- true
	}()

	if opts.All {
		go func() {
			getChunks(chunk.IndexChunk)

			indexDoneChannel <- true
		}()
		<-indexDoneChannel
	}

	<-bloomsDoneChannel
	return nil
}

type downloadFunc func(pins []manifest.PinDescriptor) (failed []manifest.PinDescriptor)

// Downloads chunks and report progress
func downloadAndReportProgress(pins []manifest.PinDescriptor, chunkType chunk.ChunkType) []manifest.PinDescriptor {
	chunkTypeToDescription := map[chunk.ChunkType]string{
		chunk.BloomChunk: "bloom",
		chunk.IndexChunk: "index",
	}
	failed := []manifest.PinDescriptor{}
	progressChannel := progress.MakeChan()
	defer close(progressChannel)

	go chunk.GetChunksFromRemote(pins, chunkType, progressChannel)

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

		switch event.Event {
		case progress.Error:
			logger.Log(logger.Error, event.Message)
			if ok {
				failed = append(failed, *pin)
			}
		case progress.Start:
			logger.Log(logger.Info, "Unchaining", chunkTypeToDescription[chunkType], event.Message, "to", fileName)
		case progress.Update:
			logger.Log(logger.Info, event.Message, fileName)
		case progress.Done:
			pinsDone++
		default:
			logger.Log(logger.Info, event.Message, fileName)
		}
	}

	return failed
}

// Retries downloading `failedPins` for `times` times by calling `downloadChunks` function.
// Returns number of pins that we were unable to fetch.
// This function is simple because: 1. it will never get a new failing pin (it only feeds in
// the list of known, failed pins); 2. The maximum number of failing pins we can get equals
// the length of `failedPins`.
func retry(failedPins []manifest.PinDescriptor, times uint, downloadChunks downloadFunc) int {
	retryCount := uint(0)

	pinsToRetry := failedPins

	for {
		if len(pinsToRetry) == 0 {
			break
		}

		if retryCount >= times {
			break
		}

		pinsToRetry = downloadChunks(pinsToRetry)

		retryCount++
	}

	return len(pinsToRetry)
}

func PrintManifestHeader() {
	// The following two values should be read from manifest.txt, however right now only TSV format
	// is available for download and it lacks this information
	// TODO: These values should be in a config file
	logger.Log(logger.Info, "hashToIndexFormatFile:", "Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2")
	logger.Log(logger.Info, "hashToBloomFormatFile:", "QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD")
	logger.Log(logger.Info, "unchainedIndexAddr:", pinlib.GetUnchainedIndexAddress())
	logger.Log(logger.Info, "manifestHashEncoding:", pinlib.GetManifestHashEncoding())
}
