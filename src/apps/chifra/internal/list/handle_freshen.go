package listPkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

// TODO: BOGUS -- USED TO BE ACCTSCRAPE2
import (
	"errors"
	"fmt"
	"io/ioutil"
	"log"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/ethereum/go-ethereum/common"
)

// AddressMonitorMap carries arrays of appearances that have not yet been written to the monitor file
type AddressMonitorMap map[common.Address]*monitor.Monitor

// MonitorUpdate stores the original 'chifra list' command line options plus
type MonitorUpdate struct {
	MaxTasks   int
	MonitorMap AddressMonitorMap
	Chain      string
	TestMode   bool
	LogLevel   int
}

func (opts *ListOptions) HandleFreshenMonitors(monitorArray *[]monitor.Monitor) error {
	var updater = MonitorUpdate{
		MaxTasks:   12,
		MonitorMap: make(AddressMonitorMap, len(opts.Addrs)),
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		LogLevel:   int(opts.Globals.LogLevel),
	}

	// This removes duplicates from the input array and keep a map from address to
	// a pointer to the monitors
	for _, addr := range opts.Addrs {
		if updater.MonitorMap[common.HexToAddress(addr)] == nil {
			m := monitor.NewStagedMonitor(opts.Globals.Chain, addr, opts.Globals.TestMode)
			m.ReadMonHeader()
			// TODO: BOGUS1
			// fmt.Fprintf(os.Stdout, "Init: %s\n", m)
			*monitorArray = append(*monitorArray, m)
			// we need the address here because we want to modify this object below
			updater.MonitorMap[m.Address] = &(*monitorArray)[len(*monitorArray)-1]
		}
	}

	chain := opts.Globals.Chain
	bloomPath := config.GetPathToIndex(chain) + "blooms/"
	files, err := ioutil.ReadDir(bloomPath)
	if err != nil {
		return err
	}

	var wg sync.WaitGroup
	resultChannel := make(chan []index.AppearanceResult, len(files))

	taskCount := 0
	for _, info := range files {
		if !info.IsDir() {
			bloomFilename := bloomPath + "/" + info.Name()
			fileRange, err := cache.RangeFromFilename(bloomFilename)
			if err != nil {
				// don't respond further -- there may be foreign files in the folder
				fmt.Println(err)
				continue
			}

			if opts.Globals.TestMode && fileRange.Last > 5000000 {
				continue
			}

			if taskCount >= updater.MaxTasks {
				resArray := <-resultChannel
				for _, r := range resArray {
					updater.updateMonitors(&r)
				}
				taskCount--
			}

			// Run a go routine for each index file
			taskCount++
			wg.Add(1)
			go updater.visitChunkToFreshenFinal(bloomFilename, resultChannel, &wg)
		}
	}

	wg.Wait()
	close(resultChannel)

	for resArray := range resultChannel {
		for _, r := range resArray {
			updater.updateMonitors(&r)
		}
	}

	return updater.moveAllToProduction()
}

// visitChunkToFreshenFinal opens one index file, searches for all the address(es) we're looking for and pushes the resultRecords
// (even if empty) down the resultsChannel.
func (updater *MonitorUpdate) visitChunkToFreshenFinal(bloomFilename string, resultChannel chan<- []index.AppearanceResult, wg *sync.WaitGroup) {
	var results []index.AppearanceResult
	defer func() {
		wg.Done()
		resultChannel <- results
	}()

	// TODO: BOGUS - Should only scan if we're not already seen the block
	var bloomHits = false
	chunk, err := index.NewChunk(bloomFilename)
	// At this point, we have a chunk with two structures: a bloom and the corresponding index
	// portion. Both file's headers have been read, but the underlying data has not. Both files
	// are still open and we are responsible to close them.
	if err != nil {
		results = append(results, index.AppearanceResult{Range: chunk.Range, Err: err})
		chunk.Close()
		return
	}

	// We first check to see if any of the monitor addresses even hit the bloom filter...
	for _, mon := range updater.MonitorMap {
		if chunk.Bloom.IsMember_New(mon.Address) {
			bloomHits = true
			break
		}
	}

	// And now we're done with the bloom (note we don't defer it, we want to close it as
	// soon as we can so we don't have too many files open unnessacarily.
	chunk.Close()

	// If nothing hit, we're done with this file...
	if !bloomHits {
		for _, mon := range updater.MonitorMap {
			results = append(results, index.AppearanceResult{Address: mon.Address, Range: chunk.Range})
		}
		return
	}

	indexFilename := index.ToIndexPath(bloomFilename)
	ok, err := establishIndexChunk(updater.Chain, indexFilename)
	if err != nil {
		results = append(results, index.AppearanceResult{Range: chunk.Range, Err: err})
		return
	}
	if !ok {
		err := errors.New("could not establish index file: " + indexFilename)
		results = append(results, index.AppearanceResult{Range: chunk.Range, Err: err})
		return
	}

	indexChunk, err := index.NewChunkData(indexFilename)
	if err != nil {
		results = append(results, index.AppearanceResult{Range: chunk.Range, Err: err})
		return
	}
	defer indexChunk.Close()

	for _, mon := range updater.MonitorMap {
		results = append(results, *indexChunk.GetAppearanceRecords(mon.Address))
	}
}

// TODO: BOGUS - This could use some integration tests
// establishIndexChunk accpets cache.FileRange, converts it to file name and finds
// correspoding CID (hash) entry in the manifest. Next, it downloads the bloom filter.
func establishIndexChunk(chain string, indexFilename string) (bool, error) {
	if file.FileExists(indexFilename) {
		return true, nil
	}

	// Open manifest
	localManifest, err := manifest.FromLocalFile(chain)
	if err != nil {
		return false, err
	}

	fileRange, _ := cache.RangeFromFilename(indexFilename)

	// Find bloom filter's CID
	var matchedPin manifest.PinDescriptor
	for _, pin := range localManifest.NewPins {
		if pin.FileName == cache.FilenameFromRange(fileRange, "") {
			matchedPin = pin
			break
		}
	}
	if matchedPin.FileName == "" {
		return false, fmt.Errorf("filename not found in pins: %s", indexFilename)
	}

	logger.Log(logger.Info, "Downloading", colors.Blue, fileRange, colors.Off, "from IPFS.")

	// Start downloading the filter
	pins := []manifest.PinDescriptor{matchedPin}
	chunkPath := cache.NewCachePath(chain, cache.Index_Final)
	progressChannel := make(chan *progress.Progress)

	go func() {
		index.GetChunksFromRemote(chain, pins, &chunkPath, progressChannel)
		close(progressChannel)
	}()

	for event := range progressChannel {
		exists := file.FileExists(indexFilename)
		switch event.Event {
		case progress.AllDone:
			return exists, nil
		case progress.Cancelled:
			return exists, nil
		case progress.Error:
			return exists, fmt.Errorf("error while downloading: %s", event.Message)
		}
	}
	return file.FileExists(indexFilename), nil
}

// updateMonitors writes an array of appearances to the Monitor file updating the header for lastScanned. It
// is called by 'chifra list' and 'chifra export' prior to reporting results
func (updater *MonitorUpdate) updateMonitors(result *index.AppearanceResult) {
	if result == nil {
		fmt.Println("Should not happen -- null result")
		return
	}

	if result.Err != nil {
		fmt.Println("Error processing index file:", result.Err)
		return
	}

	mon := updater.MonitorMap[result.Address]
	mon.Close()
	mon.WriteMonHeader(mon.Deleted, uint32(result.Range.Last)+1)
	if result.AppRecords != nil {
		if len(*result.AppRecords) > 0 {
			_, err := mon.WriteAppearances(*result.AppRecords)
			if err != nil {
				log.Println(err)
			} else if !updater.TestMode {
				bBlue := (colors.Bright + colors.Blue)
				log.Printf("%s%s%s adding appearances (count: %d)\n", bBlue, mon.GetAddrStr(), colors.Off, len(*result.AppRecords))
			}
		}
	}
}

// moveAllToProduction completes the update by moving the monitor files from ./cache/<chain>/monitors/staging to
// ./cache/<chain>/monitors.
func (updater *MonitorUpdate) moveAllToProduction() error {
	for _, mon := range updater.MonitorMap {
		err := mon.MoveToProduction()
		if err != nil {
			return err
		}
		// TODO: BOGUS1
		// fmt.Fprintf(os.Stdout, "Donze: %s%s --> %s\n", colors.Red, mon, colors.Off)
	}
	return nil
}
