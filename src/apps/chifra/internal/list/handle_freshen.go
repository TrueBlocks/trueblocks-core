package listPkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

// TODO: BOGUS -- USED TO BE ACCTSCRAPE2
import (
	"encoding/csv"
	"fmt"
	"io/ioutil"
	"log"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

func (optsEx *ListOptionsExtended) HandleFreshenMonitors() error {
	chain := optsEx.opts.Globals.Chain
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
			if taskCount >= optsEx.maxTasks {
				resArray := <-resultChannel
				for _, r := range resArray {
					optsEx.PostProcess(&r)
				}
				taskCount--
			}
			taskCount++
			indexFileName := indexPath + "/" + info.Name()
			wg.Add(1)
			go optsEx.visitChunkToFreshenFinal(indexFileName, optsEx.addrMonMap, resultChannel, &wg)
		}
	}

	wg.Wait()
	close(resultChannel)

	for resArray := range resultChannel {
		for _, r := range resArray {
			optsEx.PostProcess(&r)
		}
	}

	return nil
}

// visitChunkToFreshenFinal opens one index file, searches for all the address(es) we're looking for and pushes the resultRecords
// (even if empty) down the resultsChannel.
func (optsEx *ListOptionsExtended) visitChunkToFreshenFinal(fileName string, monitors AddressMonitorMap, resultChannel chan<- []index.ResultRecord, wg *sync.WaitGroup) {
	var results []index.ResultRecord
	defer func() {
		wg.Done()
		resultChannel <- results
	}()

	indexChunk, err := index.LoadIndexHeader(fileName)
	if err != nil {
		log.Println(err)
		return
	}
	defer indexChunk.Close()

	if optsEx.opts.Globals.TestMode && indexChunk.Range.Last > 5000000 {
		return
	}

	if !optsEx.RangesIntersect(indexChunk.Range) {
		return
	}

	for _, mon := range monitors {
		rec := indexChunk.GetAppearanceRecords(mon.Address)
		if rec != nil {
			results = append(results, *rec)
		}
	}
}

// PostProcess
func (optsEx *ListOptionsExtended) PostProcess(result *index.ResultRecord) {
	if optsEx.opts.Count {
		return
	}

	if result == nil {
		fmt.Println("null result")
		return
	}

	if result.AppRecords == nil || len(*result.AppRecords) == 0 {
		fmt.Println("Empty result record:", result.Address)
		return
	}

	// log.Println(colors.Bright, colors.Blue, result.Address, colors.Off, len(*result.AppRecords), "records.")
	// if opts.Count {
	// 	log.Println(colors.Bright, colors.Blue, result.Address, colors.Off, len(*result.AppRecords), "records")
	// }

	theWriter := csv.NewWriter(optsEx.writer)
	theWriter.Comma = 0x9
	var out [][]string
	for _, app := range *result.AppRecords {
		out = append(out, []string{strings.ToLower(result.Address.Hex()), fmt.Sprintf("%d", app.BlockNumber), fmt.Sprintf("%d", app.TransactionId)})
	}
	theWriter.WriteAll(out)
	if err := theWriter.Error(); err != nil {
		// TODO: BOGUS
		log.Fatal("F", err)
	}
}
