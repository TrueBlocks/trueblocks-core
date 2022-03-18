package listPkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

// TODO: BOGUS -- USED TO BE ACCTSCRAPE2
import (
	"encoding/binary"
	"encoding/csv"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"os"
	"sort"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/chunk"
)

func (opts *ListOptions) FreshenAndExport() {
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
				// wait until we can clear out some room in the channel
				resArray := <-resultChannel
				for _, r := range resArray {
					r.Output()
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
			r.Output()
		}
	}

	wg.Wait()
	close(resultChannel)

	for resArray := range resultChannel {
		for _, r := range resArray {
			r.Output()
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

	head, err := chunk.ReadHeader(f)
	if err != nil {
		// TODO(BOGUS): handle this panic or use an error channel?
		log.Fatal(err)
	}

	var results []ResultRecord
	for _, address := range addrs {
		searchAddress, err := chunk.NewAddressFromHex(address)
		if err != nil {
			// TODO(BOGUS): handle this panic or use an error channel?
			log.Fatal(err)
		}

		foundAt := searchForAddressRecord(f, head.AddressCount, searchAddress)
		if foundAt != -1 {
			startOfAddress := int64(HeaderSize + (foundAt * AddressSize))
			_, err = f.Seek(startOfAddress, io.SeekStart)
			if err != nil {
				// TODO(BOGUS): handle this panic or use an error channel?
				log.Fatal(err)
			}

			addressRecord, err := chunk.ReadAddressRecord(f)
			if err != nil {
				// TODO(BOGUS): handle this panic or use an error channel?
				log.Fatal(err)
			}

			appearances, err := readAppearanceRecords(f, address, head.AddressCount, addressRecord.Offset, addressRecord.Count)
			if err != nil {
				// TODO(BOGUS): handle this panic or use an error channel?
				log.Fatal(err)
			}

			results = append(results, ResultRecord{Address: address, AppRecords: &appearances})
		}
	}

	resultChannel <- results
}

func readAppearanceRecords(f *os.File, address string, numAddresses, offset, numRecs uint32) (apps []chunk.AppearanceRecord, err error) {
	readLocation := int64(HeaderSize + AddressSize*numAddresses + AppearanceSize*offset)

	_, err = f.Seek(readLocation, io.SeekStart)
	if err != nil {
		return
	}

	apps = make([]chunk.AppearanceRecord, numRecs)
	err = binary.Read(f, binary.LittleEndian, &apps)
	return
}

func searchForAddressRecord(f *os.File, numRecs uint32, addr chunk.EthAddress) int {
	compareFunc := func(pos int) bool {
		if pos == -1 {
			return false
		}

		if pos == int(numRecs) {
			return true
		}

		readLocation := int64(HeaderSize + pos*AddressSize)
		_, err := f.Seek(readLocation, io.SeekStart)
		if err != nil {
			fmt.Println(err)
			return false
		}

		addressRec, err := chunk.ReadAddressRecord(f)
		if err != nil {
			fmt.Println(err)
			return false
		}

		return addressRec.Bytes.Compare(addr) >= 0
	}

	pos := sort.Search(int(numRecs), compareFunc)

	readLocation := int64(HeaderSize + pos*AddressSize)
	f.Seek(readLocation, io.SeekStart)
	rec, err := chunk.ReadAddressRecord(f)
	if err != nil {
		return -1
	}

	if rec.Bytes.Compare(addr) != 0 {
		return -1
	}

	return pos
}

type ResultRecord struct {
	Address    string
	AppRecords *[]chunk.AppearanceRecord
}

func (result *ResultRecord) Output() {
	if result.AppRecords == nil {
		return
	}

	w := csv.NewWriter(os.Stdout)
	w.Comma = 0x9
	var out [][]string
	for _, app := range *result.AppRecords {
		out = append(out, []string{result.Address, fmt.Sprintf("%d", app.BlockNumber), fmt.Sprintf("%d", app.TransactionId)})
	}
	w.WriteAll(out)
	if err := w.Error(); err != nil {
		log.Fatal(err)
	}
}

const (
	// MagicNumber is used to check data validity
	MagicNumber = 0xdeadbeef // deadbeef in hex
	// HashLength = number of bytes in a HASH
	HashLength = 32
	// HeaderSize - size of Header Record
	HeaderSize = 44
	// AddressSize - size of Address Record
	AddressSize = 28
	// AppearanceSize - size of Appearance Record
	AppearanceSize = 8
)

type EthHash [HashLength]byte
