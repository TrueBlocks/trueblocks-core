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
	"github.com/spf13/cobra"
)

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

func runNewPins(cmd *cobra.Command, args []string) {
	url := args[0]
	target := args[1]

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

	progress := make(chan *chunk.ChunkProgress, 10)
	defer close(progress)
	go chunk.GetChunksFromRemote(m.NewPins[2300:], chunk.BloomChunk, progress)

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

		eventLabel := progressToLabel[event.Event]

		log.Printf("[%s]: %s %s\n", event.FileName, eventLabel, event.Message)
	}

}

// EXISTING_CODE
// EXISTING_CODE
