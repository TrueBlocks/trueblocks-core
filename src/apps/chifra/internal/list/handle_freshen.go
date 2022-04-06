package listPkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

// TODO: BOGUS -- USED TO BE ACCTSCRAPE2
import (
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"os"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/ethereum/go-ethereum/common"
)

// AddressMonitorMap carries arrays of appearances that have not yet been written to the monitor file
type AddressMonitorMap map[common.Address]*monitor.Monitor

// MonitorUpdate stores the original 'chifra list' command line options plus
type MonitorUpdate struct {
	MaxTasks    int
	MonitorMap  AddressMonitorMap
	Options     *ListOptions
	ExportRange cache.FileRange
	Writer      io.Writer
}

func (opts *ListOptions) HandleFreshenMonitors(monitorArray *[]monitor.Monitor) error {
	var updater = MonitorUpdate{
		MaxTasks:    12,
		MonitorMap:  make(AddressMonitorMap, len(opts.Addrs)),
		ExportRange: cache.FileRange{First: opts.FirstBlock, Last: opts.LastBlock},
		Options:     opts,
		Writer:      os.Stdout,
	}

	// This removes duplicates from the input array and keep a map from address to
	// a pointer to the monitors
	for _, addr := range opts.Addrs {
		if updater.MonitorMap[common.HexToAddress(addr)] == nil {
			m := monitor.NewStagedMonitor(opts.Globals.Chain, addr, opts.Globals.TestMode)
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
func (updater *MonitorUpdate) visitChunkToFreshenFinal(fileName string, resultChannel chan<- []index.AppearanceResult, wg *sync.WaitGroup) {
	var results []index.AppearanceResult
	defer func() {
		wg.Done()
		resultChannel <- results
	}()

	// bloom, err := index.NewChunk(fileName)
	// if err != nil {
	// 	fmt.Println("Error", fileName, err)
	// 	return
	// }
	// var bloomHits = false
	// for _, mon := range updater.MonitorMap {
	// 	if bloom.Bloom.IsMember(mon.Address) {
	// 		bloomHits = true
	// 		break
	// 	}
	// }
	// if !bloomHits {
	// 	// log.Println("Bloom filter does not hit for: ", fileName)
	// 	return
	// }

	indexChunk, err := index.NewChunkData(fileName)
	if err != nil {
		log.Println(err)
		return
	}
	defer indexChunk.Close()

	for _, mon := range updater.MonitorMap {
		rec := indexChunk.GetAppearanceRecords(mon.Address)
		if rec != nil {
			results = append(results, *rec)
		} else {
			err := mon.WriteHeader(mon.Deleted, uint32(indexChunk.Range.Last+1))
			if err != nil {
				log.Println(err)
			}
		}
	}
}

// downloadBloomByRange accpets cache.FileRange, converts it to file name and finds
// correspoding CID (hash) entry in the manifest. Next, it downloads the bloom filter.
func downloadBloomByRange(chain string, fileRange cache.FileRange) error {
	// Open manifest
	localManifest, err := manifest.FromLocalFile(chain)
	if err != nil {
		return err
	}

	// Find bloom filter's CID
	var matchedPin manifest.PinDescriptor
	chunkFileName := cache.FilenameFromRange(fileRange, "")
	for _, pin := range localManifest.NewPins {
		if pin.FileName == chunkFileName {
			matchedPin = pin
			break
		}
	}
	if matchedPin.FileName == "" {
		return fmt.Errorf("filename not found in pins: %s", chunkFileName)
	}

	// Start downloading the filter
	pins := []manifest.PinDescriptor{matchedPin}
	chunkPath := cache.NewCachePath(chain, cache.Index_Bloom)
	progressChannel := make(chan *progress.Progress)

	go func() {
		index.GetChunksFromRemote(chain, pins, &chunkPath, progressChannel)
		close(progressChannel)
	}()

	for event := range progressChannel {
		switch event.Event {
		case progress.AllDone:
			return nil
		case progress.Cancelled:
			return nil
		case progress.Error:
			return fmt.Errorf("error while downloading: %s", event.Message)
		}
	}
	return nil
}

// updateMonitors writes an array of appearances to the Monitor file updating the header for lastScanned. It
// is called by 'chifra list' and 'chifra export' prior to reporting results
func (updater *MonitorUpdate) updateMonitors(result *index.AppearanceResult) {
	mon := updater.MonitorMap[result.Address]
	if result == nil {
		fmt.Println("Should not happen -- null result")
		return
	}

	if result.AppRecords == nil || len(*result.AppRecords) == 0 {
		fmt.Println("Should not happen -- empty result record:", result.Address)
		return
	}

	_, err := mon.WriteAppearances(*result.AppRecords, uint32(result.Range.Last))
	if err != nil {
		log.Println(err)
	} else if !updater.Options.Globals.TestMode {
		bBlue := (colors.Bright + colors.Blue)
		log.Printf("Found %s%s%s adding appearances (count: %d)\n", bBlue, mon.GetAddrStr(), colors.Off, len(*result.AppRecords))
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
	}
	return nil
}
