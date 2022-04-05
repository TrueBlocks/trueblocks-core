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
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/ethereum/go-ethereum/common"
)

// AddressMonitorMap carries arrays of appearances that have not yet been written to the monitor file
type AddressMonitorMap map[common.Address]*monitor.Monitor

// MonitorUpdate stores the original 'chifra list' command line options plus
type MonitorUpdate struct {
	MaxTasks   int
	MonitorMap AddressMonitorMap
	Globals    globals.GlobalOptions
	Range      cache.FileRange
	Writer     io.Writer
}

func (opts *ListOptions) HandleFreshenMonitors(monitorArray *[]monitor.Monitor) error {
	var updater = MonitorUpdate{
		MaxTasks:   12,
		MonitorMap: make(AddressMonitorMap, len(opts.Addrs)),
		Range:      cache.FileRange{First: opts.FirstBlock, Last: opts.LastBlock},
		Globals:    opts.Globals,
		Writer:     os.Stdout,
	}

	for _, addr := range opts.Addrs {
		if updater.MonitorMap[common.HexToAddress(addr)] == nil {
			m := monitor.NewStagedMonitor(updater.Globals.Chain, addr, opts.Globals.TestMode)
			*monitorArray = append(*monitorArray, m)
			updater.MonitorMap[m.Address] = &(*monitorArray)[len(*monitorArray)-1]
		}
	}

	chain := updater.Globals.Chain
	indexPath := config.GetPathToIndex(chain) + "finalized/"
	files, err := ioutil.ReadDir(indexPath)
	if err != nil {
		return err
	}

	var wg sync.WaitGroup
	resultChannel := make(chan []index.AppearanceResult, len(files))

	taskCount := 0
	for _, info := range files {
		if !info.IsDir() {
			indexFileName := indexPath + "/" + info.Name()
			fileRange, err := cache.RangeFromFilename(indexFileName)
			if err != nil {
				// don't respond further -- there may be foreign files in the folder
				fmt.Println(err)
				continue
			}

			if updater.Globals.TestMode && fileRange.Last > 5000000 {
				continue
			}

			if !cache.Intersects(updater.Range, fileRange) {
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
			go updater.visitChunkToFreshenFinal(indexFileName, resultChannel, &wg)
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

	var bloomHits = false
	for _, mon := range updater.MonitorMap {
		bloomHits = mon.Address != common.Address{}
	}
	if !bloomHits {
		bloomFilename := strings.Replace(fileName, ".bin", ".bloom", -1)
		bloomFilename = strings.Replace(fileName, "finalized", "blooms", -1)
		log.Println("No blooms hit: ", bloomFilename)
		return
	}

	indexChunk, err := index.NewIndexData(fileName)
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
	} else if !updater.Globals.TestMode {
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
