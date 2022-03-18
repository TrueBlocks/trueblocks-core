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
	"os"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/address"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

func (opts *ListOptions) FreshenMonitor(store bool, w io.Writer) {
	indexPath := config.GetPathToIndex(opts.Globals.Chain) + "finalized/"
	files, err := ioutil.ReadDir(indexPath)
	if err != nil {
		log.Fatal(err)
	}

	var wg sync.WaitGroup
	resultChannel := make(chan []ResultRecord, len(files))

	taskCount := 0
	var maxTasks = 10
	for _, info := range files {
		if !info.IsDir() {
			if taskCount >= maxTasks {
				resArray := <-resultChannel
				for _, r := range resArray {
					r.Output(w)
				}
				taskCount--
			}
			wg.Add(1)
			taskCount++
			indexFileName := indexPath + "/" + info.Name()
			go parseFile(indexFileName, opts.Addrs, resultChannel, &wg)
		}
	}

	for i := 0; i < taskCount; i++ {
		resArray := <-resultChannel
		for _, r := range resArray {
			if store {
				log.Println("Storing ", r.Address, len(*r.AppRecords), "records")
			} else {
				log.Println("Not storing", r.Address, len(*r.AppRecords), "records")
			}
			r.Output(w)
		}
	}

	wg.Wait()
	close(resultChannel)

	for resArray := range resultChannel {
		for _, r := range resArray {
			r.Output(w)
		}
	}
}

func parseFile(fileName string, addrs []string, resultChannel chan<- []ResultRecord, wg *sync.WaitGroup) {
	defer wg.Done()

	if !file.FileExists(fileName) {
		log.Fatal(fmt.Errorf("file does not exist %s", fileName))
	}

	f, err := os.Open(fileName)
	if err != nil {
		// TODO(BOGUS): handle this panic or use an error channel?
		log.Fatal(err)
	}
	defer f.Close()

	head, err := index.ReadHeader(f)
	if err != nil {
		// TODO(BOGUS): handle this panic or use an error channel?
		log.Fatal(err)
	}

	var results []ResultRecord
	for _, addr := range addrs {
		searchAddress, err := address.Hex_2_Address(addr)
		if err != nil {
			// TODO(BOGUS): handle this panic or use an error channel?
			log.Fatal(err)
		}

		foundAt := index.SearchForAddressRecord(f, head.AddressCount, searchAddress)
		if foundAt != -1 {
			startOfAddressRecord := int64(index.HeaderWidth + (foundAt * index.AddrRecordWidth))
			_, err = f.Seek(startOfAddressRecord, io.SeekStart)
			if err != nil {
				// TODO(BOGUS): handle this panic or use an error channel?
				log.Fatal(err)
			}

			addressRecord, err := index.ReadAddressRecord(f)
			if err != nil {
				// TODO(BOGUS): handle this panic or use an error channel?
				log.Fatal(err)
			}

			appearances, err := index.ReadAppearanceRecords(f, head.AddressCount, &addressRecord)
			if err != nil {
				// TODO(BOGUS): handle this panic or use an error channel?
				log.Fatal(err)
			}

			results = append(results, ResultRecord{Address: addr, AppRecords: &appearances})
		}
	}

	resultChannel <- results
}

type ResultRecord struct {
	Address    string
	AppRecords *[]index.AppearanceRecord
}

func (result *ResultRecord) Output(w io.Writer) {
	if result.AppRecords == nil {
		return
	}

	theWriter := csv.NewWriter(w)
	theWriter.Comma = 0x9
	var out [][]string
	for _, app := range *result.AppRecords {
		out = append(out, []string{result.Address, fmt.Sprintf("%d", app.BlockNumber), fmt.Sprintf("%d", app.TransactionId)})
	}
	theWriter.WriteAll(out)
	if err := theWriter.Error(); err != nil {
		log.Fatal(err)
	}
}
