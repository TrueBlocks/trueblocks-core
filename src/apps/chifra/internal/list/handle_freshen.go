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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common"
)

// AddressMonitorMap carries arrays of appearances that have not yet been written to the monitor file
type AddressMonitorMap map[common.Address]*monitor.Monitor

// MonitorUpdate stores the original 'chifra list' command line options plus
type MonitorUpdate struct {
	MaxTasks   int
	MonitorMap AddressMonitorMap
	Options    *ListOptions
	FirstBlock uint64
}

func (opts *ListOptions) HandleFreshenMonitors(monitorArray *[]monitor.Monitor) error {
	var updater = MonitorUpdate{
		MaxTasks:   12,
		MonitorMap: make(AddressMonitorMap, len(opts.Addrs)),
		Options:    opts,
		FirstBlock: utils.NOPOS,
	}

	// This removes duplicates from the input array and keep a map from address to
	// a pointer to the monitors
	for _, addr := range opts.Addrs {
		if updater.MonitorMap[common.HexToAddress(addr)] == nil {
			mon, _ := monitor.NewStagedMonitor(opts.Globals.Chain, addr, opts.Globals.TestMode)
			mon.ReadMonHeader()
			if uint64(mon.LastScanned) < updater.FirstBlock {
				updater.FirstBlock = uint64(mon.LastScanned)
			}
			*monitorArray = append(*monitorArray, mon)
			// we need the address here because we want to modify this object below
			updater.MonitorMap[mon.Address] = &(*monitorArray)[len(*monitorArray)-1]
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
		results = append(results, index.AppearanceResult{Range: chunk.Range})
		return
	}

	indexFilename := index.ToIndexPath(bloomFilename)
	ok, err := establishIndexChunk(updater.Options.Globals.Chain, indexFilename)
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
	if mon == nil {
		// In this case, there were no errors, but all monitors were skipped. This would
		// happen, for example, due to false positives hits on the bloom filters. We want
		// to update the LastScanned values for each of these monitors. WriteAppendApps, when
		// given a nil appearance list simply updates the header. Note we update for the
		// start of the next chunk (plus 1 to current range).
		for _, mon := range updater.MonitorMap {
			// TODO: BOGUS - can we manage these files pointers so they aren't
			// copied and therefore there aren't too many of them and it crashes
			mon.Close()
			err := mon.WriteAppendApps(uint32(result.Range.Last)+1, nil)
			if err != nil {
				log.Println(err)
			}
		}

	} else {
		mon.Close()
		// Note that result.AppRecords may be nil here (because, for example, this
		// monitor had a false positive), but we do still want to update the header.
		mon.WriteMonHeader(mon.Deleted, uint32(result.Range.Last)+1)
		if result.AppRecords != nil {
			nWritten := len(*result.AppRecords)
			if nWritten > 0 {
				_, err := mon.WriteAppearances(*result.AppRecords)
				if err != nil {
					log.Println(err)
				} else if !updater.Options.Globals.TestMode {
					bBlue := (colors.Bright + colors.Blue)
					log.Printf("%s%s%s appended %d apps at %s\n", bBlue, mon.GetAddrStr(), colors.Off, nWritten, result.Range)
				}
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
