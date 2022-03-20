package listPkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

// TODO: BOGUS -- USED TO BE ACCTSCRAPE2
import (
	"encoding/csv"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
)

func (opts *ListOptions) HandleFreshenMonitors(maxTasks int, w io.Writer) error {
	indexPath := config.GetPathToIndex(opts.Globals.Chain) + "finalized/"
	files, err := ioutil.ReadDir(indexPath)
	if err != nil {
		return err
	}

	var wg sync.WaitGroup
	resultChannel := make(chan []index.ResultRecord, len(files))

	var monitors []monitor.Monitor
	for _, addr := range opts.Addrs {
		monitors = append(monitors, monitor.NewMonitor(opts.Globals.Chain, addr))
	}

	taskCount := 0
	for _, info := range files {
		if !info.IsDir() {
			if taskCount >= maxTasks {
				resArray := <-resultChannel
				for _, r := range resArray {
					opts.Report(w, &r)
				}
				taskCount--
			}
			taskCount++
			indexFileName := indexPath + "/" + info.Name()
			wg.Add(1)
			go opts.parseFile(indexFileName, monitors, resultChannel, &wg)
		}
	}

	wg.Wait()
	close(resultChannel)

	for resArray := range resultChannel {
		for _, r := range resArray {
			opts.Report(w, &r)
		}
	}
	return nil
}

// parseFile opens one index file, searches for all the address(es) we're looking for and pushes the resultRecords
// (even if empty) down the resultsChannel.
func (opts *ListOptions) parseFile(fileName string, monitors []monitor.Monitor, resultChannel chan<- []index.ResultRecord, wg *sync.WaitGroup) {
	var results []index.ResultRecord
	defer func() {
		wg.Done()
		resultChannel <- results
	}()

	indexChunk, err := index.LoadIndexChunk(fileName)
	if err != nil {
		log.Println(err)
		return
	}
	defer indexChunk.Close()

	if opts.Globals.TestMode && indexChunk.Range.Last > 5000000 {
		return
	}

	for _, mon := range monitors {
		rec := indexChunk.GetAppearanceRecords(mon.Address)
		if rec != nil {
			results = append(results, *rec)
		}
	}
}

func (opts *ListOptions) Report(w io.Writer, result *index.ResultRecord) {
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

	theWriter := csv.NewWriter(w)
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
