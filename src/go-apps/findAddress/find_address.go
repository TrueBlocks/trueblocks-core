package main

import (
	"bytes"
	"encoding/csv"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"strings"
	"sync"

	"github.com/tushar2708/altcsv"
	"gopkg.in/natefinch/lumberjack.v2"

	flag "github.com/spf13/pflag"
	"github.com/spf13/viper"
)

const (
	// MagicNumber is used to check data validity
	MagicNumber = 3735928559 // deadbeef
	// AddressLength = number of bytes of an address
	AddressLength = 20
	// HashLength = number of bytes in a HASH
	HashLength = 32

	// HeaderSize - size of Header Record
	HeaderSize = 44
	// AddressSize - size of Address Record
	AddressSize = 28
	// AppearanceSize - size of Appearance Record
	AppearanceSize = 8
)

// Address is 20 bytes
type Address [AddressLength]byte

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

func (a Address) compare(b Address) int {
	return bytes.Compare(a[:], b[:])
}

func (a *Address) setBytes(b []byte) {
	if len(b) > len(a) {
		b = b[len(b)-AddressLength:]
	}
	copy(a[AddressLength-len(b):], b)
}

func newAddressFromHex(hexString string) (newAddress Address, err error) {
	if hexString[:2] == "0x" {
		hexString = hexString[2:]
	}
	hexBytes, err := hex.DecodeString(hexString)
	if err != nil {
		return Address{}, err
	}
	newAddress.setBytes(hexBytes)
	return
}

func writeTextTabbedCSV(data [][]string) {
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
		out = append(out, []string{addrStr, fmt.Sprint(rec.Blk), fmt.Sprint(rec.Txid)})
	}
	return
}

func usage() {
	fmt.Println("usage $ findAddress directory address")
	fmt.Println("option :")
	fmt.Println(" --fmt    : set output mode to : json, txt or csv")
	fmt.Println(" --max    : set maximum number of parallel threads*")
	fmt.Println(" --log    : enable logging")
	fmt.Println(" --stream : stream output from threads as received")
	fmt.Println("examples : ")
	fmt.Println("$ findAddress data/ 0xff488fd296c38a24cccc60b43dd7254810dab64e")
	fmt.Println("$ findAddress --fmt json data/ 0xff488fd296c38a24cccc60b43dd7254810dab64e")
	fmt.Println("$ findAddress --fmt csv  data/ 0xff488fd296c38a24cccc60b43dd7254810dab64e")
	fmt.Println("$ findAddress --fmt txt  data/ 0xff488fd296c38a24cccc60b43dd7254810dab64e")
	fmt.Println()
	fmt.Println("* precedence : flag > config > default (4)")
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
		writeTextTabbedCSV(convertTo2DStringArray(r))
	case "csv":
		writeQuotedCSV(convertTo2DStringArray(r))
	default:
	}
}

func main() {
	viper.SetConfigName("config")
	viper.AddConfigPath(".")
	viper.SetConfigType("toml")
	err := viper.ReadInConfig()

	var wg sync.WaitGroup
	var results []appearanceRecord
	logging := flag.Bool("log", false, "enable logging")
	format := flag.String("fmt", "json", "set output mode to : json, txt or csv")
	flag.Int("max", 4, "maximum number of parallel threads")
	streaming := flag.Bool("stream", false, "stream output from threads as received")
	flag.Parse()
	maxTasks := viper.GetInt("max")
	if *logging {
		log.SetOutput(&lumberjack.Logger{
			Filename:   "logs/findAddress.log",
			MaxSize:    500,
			MaxBackups: 3,
			MaxAge:     28,
		})
		log.Println("New Scan")
	}
	flag.Usage = usage
	*format = strings.ToLower(*format)
	switch *format {
	case "json":
		if *streaming {
			fmt.Print("[")
		}
	case "txt":
		fmt.Println("address\tblockNumber\ttransactionIndex")
	case "csv":
		fmt.Println(`"address","blockNumber","transactionIndex"`)
	default:
		flag.Usage()
		if *logging {
			log.Println("invalid flag : fmt = ", format)
		}
		os.Exit(1)
	}
	args := flag.Args()
	if len(args) != 1 {
		flag.Usage()
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
	searchAddress, err := newAddressFromHex(addrStr)
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
			if *streaming {
				streamOut(*format, r)
			} else {
				results = append(results, r...)
			}
			count--
		}
		wg.Add(1)
		count++
		go parseFile(directory+"/"+info.Name(), searchAddress, resultChannel, &wg, *logging)
	}
	for j := 0; j < count; j++ {
		select {
		case r := <-resultChannel:
			if *streaming {
				streamOut(*format, r)
			} else {
				results = append(results, r...)
			}
		}
	}
	wg.Wait()
	close(resultChannel)
	for r := range resultChannel {
		if *streaming {
			streamOut(*format, r)
		} else {
			results = append(results, r...)
		}
	}
	if !*streaming {
		switch *format {
		case "json":
			if len(results) == 0 || results == nil {
				fmt.Println("[]")
				break
			}
			err = json.NewEncoder(os.Stdout).Encode(results)
			if err != nil {
				log.Fatal(err)
			}
		case "txt":
			writeTextTabbedCSV(convertTo2DStringArray(results))
		case "csv":
			writeQuotedCSV(convertTo2DStringArray(results))
		default:
		}
	}
	if *streaming && *format == "json" {
		fmt.Println("]")
	}
	if *logging {
		log.Println("Done")
	}
}
