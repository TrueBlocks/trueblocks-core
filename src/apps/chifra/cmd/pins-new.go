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
/*
 * Parts of this file were generated with makeClass --gocmds. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package cmd

import (
	"fmt"
	"log"
	"os"
	"runtime/pprof"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/chunk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/spf13/cobra"
)

type downloadFunc func(pins []manifest.PinDescriptor) (failed []manifest.PinDescriptor)

// pinsCmd represents the pins command
var pinsNewCmd = &cobra.Command{
	Use: "pins-new [flags]",
	Run: runNewPins,
}

var pinsNewOptions struct {
	url    string
	target string
}

func init() {
	pinsNewCmd.SetOut(os.Stderr)

	pinsNewCmd.Flags().SortFlags = false
	pinsNewCmd.PersistentFlags().SortFlags = false

	pinsNewCmd.Flags().StringVar(&pinsNewOptions.url, "url", "", "what")
	pinsNewCmd.Flags().StringVar(&pinsNewOptions.target, "target", "", "where")

	rootCmd.AddCommand(pinsNewCmd)
}

func downloadAndReportProgress(pins []manifest.PinDescriptor, chunkType chunk.ChunkType) []manifest.PinDescriptor {
	failed := []manifest.PinDescriptor{}
	progress := make(chan *chunk.ChunkProgress, 100)
	defer close(progress)

	go chunk.GetChunksFromRemote(pins, chunkType, progress)

	progressToLabel := map[chunk.ProgressEvent]string{
		chunk.ProgressDownloading: "Downloading",
		chunk.ProgressUnzipping:   "Unzipping",
		chunk.ProgressValidating:  "Validating",
		chunk.ProgressError:       "Error",
	}

	for event := range progress {
		if event.Event == chunk.ProgressAllDone {
			break
		}

		if event.Event == chunk.ProgressError {
			failed = append(failed, *event.Pin)
		}

		eventLabel := progressToLabel[event.Event]

		log.Printf("[%s]: %s %s\n", event.Pin.FileName, eventLabel, event.Message)
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
		log.Println("[Retry]", "got", len(pinsToRetry), "failing")

		retryCount++
	}

	return len(pinsToRetry)
}

func runNewPins(cmd *cobra.Command, args []string) {
	toDownload := args[0]
	url := args[1]
	target := args[2]

	var chunkType chunk.ChunkType
	switch toDownload {
	case "blooms":
		chunkType = chunk.BloomChunk
	case "indexes":
		chunkType = chunk.IndexChunk
	default:
		log.Fatalln("Unsupported chunk type", toDownload)
	}

	// TODO: profiling code, remove before merging
	f, err := os.Create("profile.prof")
	if err != nil {
		log.Fatal(err)
	}
	pprof.StartCPUProfile(f)
	defer pprof.StopCPUProfile()

	m, err := pinlib.DownloadManifest(url)

	if err != nil {
		fmt.Printf("Error while downloading manifest:\n%s", err)
	}

	err = pinlib.SaveManifest(target, m)

	if err != nil {
		fmt.Printf("Error while saving manifest to %s:\n%s", target, err)
	}

	failedChunks := downloadAndReportProgress(m.NewPins, chunkType)

	if len(failedChunks) > 0 {
		retry(failedChunks, 3, func(pins []manifest.PinDescriptor) []manifest.PinDescriptor {
			log.Println("Retrying", len(pins), "pin(s)")
			return downloadAndReportProgress(pins, chunkType)
		})
	}
}

// EXISTING_CODE
// EXISTING_CODE
