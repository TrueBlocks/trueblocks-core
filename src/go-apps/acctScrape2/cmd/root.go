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
package cmd

import (
	"encoding/binary"
	"encoding/csv"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"sort"
	"strings"
	"sync"

	//"github.com/spf13/cobra"
	//homedir "github.com/mitchellh/go-homedir"

	"github.com/tushar2708/altcsv"
	"gopkg.in/natefinch/lumberjack.v2"

	"github.com/gookit/color"
	flag "github.com/spf13/pflag"
	"github.com/spf13/viper"
)

const (
	// AddressLength = number of bytes of an address
	AddressLength = 20
	// MagicNumber is used to check data validity
	MagicNumber = 3735928559 // deadbeef in hex
	// HashLength = number of bytes in a HASH
	HashLength = 32
	// HeaderSize - size of Header Record
	HeaderSize = 44
	// AddressSize - size of Address Record
	AddressSize = 28
	// AppearanceSize - size of Appearance Record
	AppearanceSize = 8
)

// EthHash : 256 bit number - not actually used
type EthHash [HashLength]byte

// Header of the File
type Header struct {
	Magic               uint32
	Hash                EthHash
	NumberOafAddresses  uint32
	NumberOfAppearances uint32
}

type addressRecord struct {
	Bytes  Address
	Offset uint32
	Cnt    uint32
}

type appearanceRecord struct {
	Blk  uint32 `json:"blockNumber"`
	Txid uint32 `json:"transactionIndex"`
}

// used for the "on the fly" insertion of address field
type fakeAppearanceRec appearanceRecord

var addrStr string

func (a appearanceRecord) MarshalJSON() ([]byte, error) {
	return json.Marshal(struct {
		Addy string `json:"address"`
		fakeAppearanceRec
	}{

		fakeAppearanceRec: fakeAppearanceRec(a),
		Addy:              addrStr,
	})
}

// This is not used : illustrates the on the fly unmarshalling output format
type modifiedAppearanceRecord struct {
	Address string `json:"address"`
	Blk     uint32 `json:"blockNumber"`
	Txid    uint32 `json:"transactionIndex"`
}

func writeTabbedText(data [][]string) {
	w := csv.NewWriter(os.Stdout)
	w.Comma = 0x9
	w.WriteAll(data)
	if err := w.Error(); err != nil {
		log.Fatal(err)
	}
}

func writeQuotedCSV(data [][]string) {
	w := altcsv.NewWriter(os.Stdout)
	w.AllQuotes = true
	w.WriteAll(data)
	if err := w.Error(); err != nil {
		log.Fatal(err)
	}
}

func convertTo2DStringArray(in []appearanceRecord) (out [][]string) {
	for _, rec := range in {
		out = append(out, []string{addrStr, fmt.Sprintf("%09d", rec.Blk), fmt.Sprintf("%09d", rec.Txid)})
	}
	return
}

// Execute The main executable function
func Execute() {
	viper.SetConfigName("config")
	viper.AddConfigPath(".")
	viper.SetConfigType("toml")
	err := viper.ReadInConfig()

	var wg sync.WaitGroup
	logging := flag.Bool("log", false, "enable logging")
	format := flag.String("fmt", "json", "set output mode to : json, txt or csv")
	flag.Int("max", 4, "maximum number of parallel threads")
	flag.Parse()
	maxTasks := viper.GetInt("max")
	if *logging {
		log.SetOutput(&lumberjack.Logger{
			Filename:   "logs/acctScrape2.log",
			MaxSize:    500,
			MaxBackups: 3,
			MaxAge:     28,
		})
		log.Println("New Scan")
	}
	*format = strings.ToLower(*format)
	switch *format {
	case "json":
		fmt.Print("[")
	case "txt":
		fmt.Println("address\tblockNumber\ttransactionIndex")
	case "csv":
		fmt.Println(`"address","blockNumber","transactionIndex"`)
	default:
		usage("What")
		if *logging {
			log.Println("invalid flag : fmt = ", format)
		}
		os.Exit(1)
	}
	args := flag.Args()
	if len(args) != 1 {
		usage("What")
		if *logging {
			log.Println("wrong number of arguments (should be 1) ", len(args))
		}
		os.Exit(1)
	}
	directory := viper.GetString("data")
	addrStr = args[0]
	if *logging {
		log.Println("looking for :", addrStr)
	}

	addrStr := args[0]
	files, err := ioutil.ReadDir(directory)
	if err != nil {
		log.Fatal(err)
	}
	resultChannel := make(chan []appearanceRecord, len(files))
	searchAddress, err := NewAddressFromHex(addrStr)
	if err != nil {
		log.Fatal(err)
	}
	count := 0
	for _, info := range files {
		if info.IsDir() {
			continue
		}
		if count >= maxTasks {
			// wait for one goroutine to finish
			r := <-resultChannel
			streamOut(*format, r)
			count--
		}
		wg.Add(1)
		count++
		go parseFile(directory+"/"+info.Name(), searchAddress, resultChannel, &wg, *logging)
	}
	for j := 0; j < count; j++ {
		select {
		case r := <-resultChannel:
			streamOut(*format, r)
		}
	}
	wg.Wait()
	close(resultChannel)
	for r := range resultChannel {
		streamOut(*format, r)
	}
	if *format == "json" {
		fmt.Println("]")
	}
	if *logging {
		log.Println("Done")
	}
}

func colr(c, s string) string {
	col := os.Getenv("NO_COLOR") != "true"
	if col {
		if c == "yellow" {
			return color.FgYellow.Render(s)
		} else {
			return color.LightBlue.Render(s)
		}
	}
	return s
}
func usage(s string) {
	fmt.Println("")
	fmt.Println(" ", colr("yellow", "Usage:"), "    acctScrape [-v|-h] <address> [address...]")
	fmt.Println(" ", colr("yellow", "Purpose:"), "  Add or remove monitors for a given Ethereum address (or collection of addresses).")
	fmt.Println("")
	fmt.Println(" ", colr("yellow", "Where:"))
	fmt.Println("        addrs                 one or more Ethereum addresses (required)")
	fmt.Println("        -m  (--max)           set maximum number of parallel threads")
	fmt.Println("        -v  (--verbose)       set verbose level (optional level defaults to 1)")
	fmt.Println("        -h  (--help)          display this help screen")
	fmt.Println("")
	fmt.Println(" ", colr("blue", "Powered by TrueBlocks (GHC-TrueBlocks//0.8.3-alpha-8af508185-20210126)"))
}

var notFirst bool // defaults to false

func streamOut(format string, r []appearanceRecord) {
	switch format {
	case "json":
		for _, r := range r {
			if notFirst {
				fmt.Println(",")
			}
			err := json.NewEncoder(os.Stdout).Encode(r)
			if err != nil {
				log.Fatal(err)
			}
			notFirst = true
		}
	case "txt":
		writeTabbedText(convertTo2DStringArray(r))
	case "csv":
		writeQuotedCSV(convertTo2DStringArray(r))
	default:
	}
}

func getHeader(f *os.File) (head Header, err error) {
	err = binary.Read(f, binary.LittleEndian, &head)
	return
}

func getAddressRecord(f *os.File) (addressRec addressRecord, err error) {
	err = binary.Read(f, binary.LittleEndian, &addressRec)
	return
}

func getAddressRecordAndCompare(f *os.File, target Address) (bigger bool, err error) {
	addressRec, err := getAddressRecord(f)
	if err != nil {
		return false, err
	}

	return addressRec.Bytes.Compare(target) >= 0, nil
}

func acctScrape2(numRecs uint32, addr Address, f *os.File) (pos int) {

	comp := func(pos int) bool {
		//fmt.Println(pos)
		if pos == -1 {
			return false
		}
		if pos == int(numRecs) {
			return true
		}
		f.Seek(int64(HeaderSize+pos*AddressSize), 0)
		bigger, err := getAddressRecordAndCompare(f, addr)
		if err != nil {
			fmt.Println(err)
			return false
		}
		return bigger
	}
	pos = sort.Search(int(numRecs), comp)
	f.Seek(int64(HeaderSize+pos*AddressSize), 0)
	rec, err := getAddressRecord(f)
	if err != nil {
		return -1
	}
	if rec.Bytes.Compare(addr) != 0 {
		return -1
	}
	return
}

func getAppearanceRecords(f *os.File, numAddresses uint32, offset uint32, numRecs uint32) (appy []appearanceRecord, err error) {
	_, err = f.Seek(int64(HeaderSize+numAddresses*AddressSize+offset*AppearanceSize), 0)
	if err != nil {
		log.Fatal(err)
	}
	appy = make([]appearanceRecord, numRecs)

	err = binary.Read(f, binary.LittleEndian, &appy)

	return
}

func parseFile(fileName string, addr Address, resultChannel chan []appearanceRecord, wg *sync.WaitGroup, logging bool) {
	defer wg.Done()
	if logging {
		log.Println(fileName)
	}
	f, err := os.Open(fileName)
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()
	head, err := getHeader(f)
	if err != nil {
		log.Fatal(err)
	}
	if head.Magic != MagicNumber {
		log.Fatal("invalid format", fileName)
	}
	var results []appearanceRecord
	newPos := acctScrape2(head.NumberOafAddresses, addr, f)
	if newPos == -1 {
		resultChannel <- results // empty array
		return
	}
	//fmt.Println(newPos)
	_, err = f.Seek(int64(HeaderSize+newPos*AddressSize), 0)
	if err != nil {
		log.Fatal(err)
	}
	adx, err := getAddressRecord(f)
	//fmt.Printf("0x%x : %d   %d\n", adx.Bytes, adx.Cnt, adx.Offset)

	results, err = getAppearanceRecords(f, head.NumberOafAddresses, adx.Offset, adx.Cnt)

	if err != nil {
		log.Fatal(err)
	}
	resultChannel <- results

	return
}
