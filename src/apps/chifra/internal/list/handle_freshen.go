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
	writer     io.Writer
	maxTasks   int
	monitorMap AddressMonitorMap
	Globals    globals.GlobalOptions
	Range      cache.FileRange
}

func (opts *ListOptions) HandleFreshenMonitors(monitorArray *[]monitor.Monitor) error {
	var updater MonitorUpdate
	updater.writer = os.Stdout
	updater.maxTasks = 12
	updater.monitorMap = make(AddressMonitorMap, len(opts.Addrs))
	updater.Range = cache.FileRange{First: opts.FirstBlock, Last: opts.LastBlock}
	updater.Globals = opts.Globals
	for _, addr := range opts.Addrs {
		if updater.monitorMap[common.HexToAddress(addr)] == nil {
			m := monitor.NewStagedMonitor(updater.Globals.Chain, addr)
			*monitorArray = append(*monitorArray, m)
			updater.monitorMap[m.Address] = &(*monitorArray)[len(*monitorArray)-1]
		}
	}

	chain := updater.Globals.Chain
	indexPath := config.GetPathToIndex(chain) + "finalized/"
	files, err := ioutil.ReadDir(indexPath)
	if err != nil {
		return err
	}

	var wg sync.WaitGroup
	resultChannel := make(chan []index.ResultRecord, len(files))

	taskCount := 0
	for _, info := range files {
		if !info.IsDir() {
			indexFileName := indexPath + "/" + info.Name()
			fR, err := cache.RangeFromFilename(indexFileName)
			if err != nil {
				// don't respond further -- there may be foreign files in the folder
				fmt.Println(err)
				continue
			}

			if updater.Globals.TestMode && fR.Last > 5000000 {
				continue
			}

			if !cache.Intersects(updater.Range, fR) {
				continue
			}

			if taskCount >= updater.maxTasks {
				resArray := <-resultChannel
				for _, r := range resArray {
					updater.updateMonitors(&r)
				}
				taskCount--
			}
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
func (updater *MonitorUpdate) visitChunkToFreshenFinal(fileName string, resultChannel chan<- []index.ResultRecord, wg *sync.WaitGroup) {
	var results []index.ResultRecord
	defer func() {
		wg.Done()
		resultChannel <- results
	}()

	indexChunk, err := index.NewIndexChunk(fileName)
	if err != nil {
		log.Println(err)
		return
	}
	defer indexChunk.Close()

	for _, mon := range updater.monitorMap {
		rec := indexChunk.GetAppearanceRecords(mon.Address)
		if rec != nil {
			results = append(results, *rec)
		} else {
			err := mon.WriteHeader(mon.Deleted, uint32(indexChunk.Range.Last))
			if err != nil {
				log.Println(err)
			}
		}
	}
}

// updateMonitors writes an array of appearances to the Monitor file updating the header for lastScanned. It
// is called by 'chifra list' and 'chifra export' prior to reporting results
func (updater *MonitorUpdate) updateMonitors(result *index.ResultRecord) {
	mon := updater.monitorMap[result.Address]
	if result == nil {
		fmt.Println("Should not happen -- null result")
		return
	}

	if result.AppRecords == nil || len(*result.AppRecords) == 0 {
		fmt.Println("Should not happen -- empty result record:", result.Address)
		return
	}

	_, err := mon.WriteApps(*result.AppRecords, uint32(result.Range.Last))
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
	for _, mon := range updater.monitorMap {
		err := mon.MoveToProduction()
		if err != nil {
			return err
		}
	}
	return nil
}
