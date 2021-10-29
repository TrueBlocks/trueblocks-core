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
package cmd

import (
	"encoding/json"
	"fmt"
	"net/url"
	"os"
	"path"
	"sort"
	"strconv"
	"strings"
	"text/tabwriter"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/chunk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

type downloadFunc func(pins []manifest.PinDescriptor) (failed []manifest.PinDescriptor)

func validatePinsArgs(cmd *cobra.Command, args []string) error {
	list := PinsOpts.list
	init := PinsOpts.init
	freshen := PinsOpts.freshen
	remote := PinsOpts.remote
	all := PinsOpts.all
	init_all := PinsOpts.init_all

	if list && (init || freshen) {
		return validate.Usage("Please choose only a single option.")
	}

	if !list && !init && !freshen {
		return validate.Usage("You must choose at least one of --list, --init, or --freshen.")
	}

	if remote && !list {
		return validate.Usage("The --remote option is only available with the --list option")
	}

	if remote {
		return validate.Usage("The --remote option is not yet implemented")
	}

	if all && !init {
		return validate.Usage("Use the --all option only with the --init or --freshen options.")
	}

	if init_all {
		return validate.Usage("Flag --init_all has been deprecated, use --init --all instead")
	}

	// if (share) {
	//     string_q res := doCommand("which ipfs");
	//     if (res.empty()) {
	//         return usage("Could not find ipfs in your $PATH. You must install ipfs for the --share command to work.");
	// 	}
	// }

	err := validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}

	return nil
}

func handleList(format string) {
	// Load manifest
	manifestData, err := manifest.FromLocalFile()
	if err != nil {
		logger.Fatal("Cannot open manifest file:", err)
	}

	// Sort pins
	sort.Slice(manifestData.NewPins, func(i, j int) bool {
		iPin := manifestData.NewPins[i]
		jPin := manifestData.NewPins[j]
		iKey := strings.Split(iPin.FileName, "-")[0]
		jKey := strings.Split(jPin.FileName, "-")[0]

		iNumber, err := strconv.ParseInt(iKey, 10, 64)
		if err != nil {
			return false
		}

		jNumber, err := strconv.ParseInt(jKey, 10, 64)
		if err != nil {
			return false
		}

		return iNumber < jNumber
	})

	// dszlachta: I think we should move printing in the given format to a package
	if format == "api" {
		response := map[string][]manifest.PinDescriptor{
			"data": manifestData.NewPins,
		}
		marshalled, err := json.MarshalIndent(response, "", "    ")
		if err != nil {
			logger.Fatal(err)
		}

		fmt.Println(string(marshalled))

		return
	}

	writer := tabwriter.NewWriter(os.Stdout, 0, 0, 1, ' ', 0)
	fmt.Fprintf(writer, "filename\tbloomhash\tindexhash\n")
	for _, pin := range manifestData.NewPins {
		fmt.Fprintf(writer, "%s\t%s\t%s\n", pin.FileName, pin.BloomHash, pin.IndexHash)
	}
	writer.Flush()
}

// Downloads chunks and report progress
func downloadAndReportProgress(pins []manifest.PinDescriptor, chunkType chunk.ChunkType) []manifest.PinDescriptor {
	chunkTypeToDescription := map[chunk.ChunkType]string{
		chunk.BloomChunk: "bloom",
		chunk.IndexChunk: "index",
	}
	failed := []manifest.PinDescriptor{}
	progress := make(chan *chunk.ChunkProgress, 100)
	defer close(progress)

	go chunk.GetChunksFromRemote(pins, chunkType, progress)

	progressToLabel := map[chunk.ProgressEvent]string{
		chunk.ProgressDownloading: "Unchaining",
		chunk.ProgressUnzipping:   "Unzipping",
		chunk.ProgressValidating:  "Validating",
		chunk.ProgressError:       "Error",
	}

	for event := range progress {
		if event.Event == chunk.ProgressAllDone {
			logger.Log(logger.Info, event.Message, "pin(s) were (re)initialized")
			break
		}

		if event.Event == chunk.ProgressError {
			failed = append(failed, *event.Pin)
		}

		if event.Event == chunk.ProgressDownloading {
			logger.Log(logger.Info, "Unchaining", chunkTypeToDescription[chunkType], event.Message, "to", event.Pin.FileName)
			continue
		}

		eventLabel := progressToLabel[event.Event]

		logger.Log(logger.Info, eventLabel, event.Pin.FileName, event.Message)
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

func printManifestHeader() {
	// The following two values should be read from manifest.txt, however right now only TSV format
	// is available for download and it lacks this information
	logger.Log(logger.Info, "hashToIndexFormatFile:", "Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2")
	logger.Log(logger.Info, "hashToBloomFormatFile:", "QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD")
	logger.Log(logger.Info, "unchainedIndexAddr:", pinlib.GetUnchainedIndexAddress())
	logger.Log(logger.Info, "manifestHashEncoding:", pinlib.GetManifestHashEncoding())
}

func handleInit() {
	logger.Log(logger.Info, "Calling unchained index smart contract...")

	// Fetch manifest's CID
	cid, err := pinlib.GetManifestCidFromContract()
	if err != nil {
		logger.Fatal(err)
	}
	logger.Log(logger.Info, "Found manifest hash at", cid)

	// Download the manifest
	gatewayUrl := config.ReadBlockScrape().Dev.IpfsGateway
	logger.Log(logger.Info, "IPFS gateway", gatewayUrl)

	url, err := url.Parse(gatewayUrl)
	if err != nil {
		logger.Fatal(err)
	}
	url.Path = path.Join(url.Path, cid)
	downloadedManifest, err := pinlib.DownloadManifest(url.String())

	if err != nil {
		fmt.Printf("Error while downloading manifest:\n%s", err)
	}

	// Save manifest
	err = pinlib.SaveManifest(config.GetConfigPath("manifest/manifest.txt"), downloadedManifest)
	if err != nil {
		fmt.Println("Error while saving manifest", err)
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

	if PinsOpts.all {
		go func() {
			getChunks(chunk.IndexChunk)

			indexDoneChannel <- true
		}()
		<-indexDoneChannel
	}

	<-bloomsDoneChannel
}

func runPins(cmd *cobra.Command, args []string) {
	if PinsOpts.list {
		printManifestHeader()
		handleList(RootOpts.fmt)
		return
	}
	if PinsOpts.init || PinsOpts.freshen {
		printManifestHeader()
		handleInit()
		return
	}
	if PinsOpts.remote {
		logger.Fatal("Not implemented")
	}
	if PinsOpts.sleep != .25 {
		logger.Fatal("Not implemented")
	}
	if PinsOpts.share {
		logger.Fatal("Not implemented")
	}
}
