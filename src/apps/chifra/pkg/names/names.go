package names

import (
	"encoding/binary"
	"encoding/csv"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// Name is a record in the names database
type Name struct {
	Tags       string `json:"tags"`
	Address    string `json:"address"`
	Name       string `json:"name"`
	Symbol     string `json:"symbol"`
	Source     string `json:"source"`
	Decimals   string `json:"decimals"`
	Petname    string `json:"petname"`
	Deleted    bool   `json:"deleted"`
	IsCustom   bool   `json:"isCustom"`
	IsPrefund  bool   `json:"isPrefund"`
	IsContract bool   `json:"isContract"`
	IsErc20    bool   `json:"isErc20"`
	IsErc721   bool   `json:"isErc721"`
}

func (n Name) String() string {
	ret, _ := json.MarshalIndent(n, "", "  ")
	return string(ret)
}

type NamesArray []Name

// NameOnDisc is a record in the names database when stored in the binary backing file
type NameOnDisc struct {
x
	Tags     [30 + 1]byte  `json:"-"` // 30 bytes
	Address  [42 + 1]byte  `json:"-"` // + 42 bytes = 72 bytes
	Name     [120 + 1]byte `json:"-"` // + 121 bytes = 193 bytes
	Symbol   [30 + 1]byte  `json:"-"` // + 31 bytes = 224 bytes
	Source   [180 + 1]byte `json:"-"` // + 181 bytes = 405 bytes
	Petname  [40]byte      `json:"-"` // + 41 bytes = 446 bytes
	Decimals [2]byte       `json:"-"` // + 2 bytes = 448 bytes
	Flags    [2]byte       `json:"-"` // + 2 bytes = 450 bytes
	Padding  byte          `json:"-"` // + 1 bytes = 451 bytes
}

type NamesMap map[types.Address]Name

type NameOnDiscHeader struct {
	Magic   uint64    // 8 bytes
	Version uint64    // + 8 bytes = 16 bytes
	Count   uint64    // + 8 bytes = 24 bytes
	Padding [428]byte // 451 - 24 = 427 bytes
}

type Parts int

const (
	None      Parts = 0x0
	Regular   Parts = 0x1
	Custom    Parts = 0x2
	Prefund   Parts = 0x4
	Testing   Parts = 0x8
	MatchCase Parts = 0x10
	Expanded  Parts = 0x20
)

type SortBy int

const (
	SortByAddress SortBy = iota
	SortByName
	// SortBySymbol
	// SortBySource
	// SortByDecimals
	SortByTags
	// SortByPetname
)

func LoadNamesArray(chain string, parts Parts, sortBy SortBy, terms []string) (NamesArray, error) {

	names := NamesArray{}
	if namesMap, err := LoadNamesMap(chain, parts, terms); err != nil {
		return nil, err
	} else {
		for _, name := range namesMap {
			isTesting := parts&Testing != 0
			isIndiv := strings.Contains(name.Tags, "Individual")
			if name.Address == "0x69e271483c38ed4902a55c3ea8aab9e7cc8617e5" {
				isIndiv = false
				name.Name = "Name 0x69e27148"
			}
			if !isTesting || !isIndiv {
				names = append(names, name)
			}
		}
	}

	sort.Slice(names, func(i, j int) bool {
		switch sortBy {
		case SortByName:
			return names[i].Name < names[j].Name
		case SortByTags:
			return names[i].Tags < names[j].Tags
		case SortByAddress:
			fallthrough
		default:
			return names[i].Address < names[j].Address
		}
	})

	isTesting := parts&Testing != 0
	isTags := sortBy == SortByTags
	if isTesting && !isTags {
		names = names[:utils.Min(200, len(names))]
	}

	return names, nil
}

func LoadNamesMap(chain string, parts Parts, terms []string) (NamesMap, error) {
	binPath := config.GetPathToCache(chain) + "names/names.bin"
	if file.FileExists(binPath) {
		file, _ := os.OpenFile(binPath, os.O_RDONLY, 0)
		defer file.Close()

		header := NameOnDiscHeader{}
		err := binary.Read(file, binary.LittleEndian, &header)
		if err != nil {
			return nil, err
		}
		// fmt.Println(thing)

		ret := NamesMap{}
		for i := uint64(0); i < thing.Count; i++ {
			v := NameOnDisc{}
			binary.Read(file, binary.LittleEndian, &v)
			n := Name{
				Tags:    asString("tags", v.Tags[:]),
				Address: asString("address", v.Address[:]),
				Name:    asString("name", v.Name[:]),
				Symbol:  asString("symbol", v.Symbol[:]),
x				// Decimals: asString("decimals", v.Decimals),
				Source:  asString("source", v.Source[:]),
				Petname: asString("petname", v.Petname[:]),
			}
			if doSearch(n, terms, parts) {
				ret[types.HexToAddress(n.Address)] = n
			}
		}
		return ret, nil
	}

	ret := NamesMap{}
	if parts&Regular != 0 {
		nameMapFromFile(chain, &ret, "names.tab", terms, parts)
	}

	if parts&Custom != 0 && parts&Testing != 0 {
		getCustomTestNames(&ret, terms, parts)
	} else if parts&Custom != 0 {
		nameMapFromFile(chain, &ret, "names_custom.tab", terms, parts)
	}

	if parts&Prefund != 0 {
		nameMapFromPrefund(chain, &ret, terms, parts)
	}

	return ret, nil
}

func nameMapFromPrefund(chain string, ret *NamesMap, terms []string, parts Parts) {
	prefunds, _ := LoadPrefunds(chain)
	for i, prefund := range prefunds {
		n := Name{
			Tags:      "80-Prefund",
			Address:   prefund.Address.Hex(),
			Name:      "Prefund_" + fmt.Sprintf("%04d", i),
			Source:    "Genesis",
			Petname:   AddrToPetname(prefund.Address.Hex(), "-"),
			IsPrefund: true,
		}
		if doSearch(n, terms, parts) {
			(*ret)[types.HexToAddress(n.Address)] = n
		}
	}
}

func nameMapFromFile(chain string, ret *NamesMap, filename string, terms []string, parts Parts) {
	namesPath := filepath.Join(config.GetPathToChainConfig(chain), filename)
	gr, err := NewNameReader(namesPath)
	if err != nil {
		log.Fatal(err)
	}

	for {
		n, err := gr.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			log.Fatal(err)
		}
		if doSearch(n, terms, parts) {
			(*ret)[types.HexToAddress(n.Address)] = n
		}
	}
}

// asString converts the byte array (not zero-terminated) to a string
func asString(which string, b []byte) string {
	ret := ""
	for i := 0; i < len(b); i++ {
		if b[i] != 0 {
			ret += string(b[i])
		} else {
			return ret
		}
	}
	return ret
}

var requiredColumns = []string{
	"tags",
	"address",
	"name",
	"symbol",
	"source",
	"petname",
}

type NameReader struct {
	file      *os.File
	header    map[string]int
	csvReader csv.Reader
}

func (gr *NameReader) Read() (Name, error) {
	record, err := gr.csvReader.Read()
	if err == io.EOF {
		gr.file.Close()
	}
	if err != nil {
		return Name{}, err
	}

	return Name{
		Tags:       record[gr.header["tags"]],
		Address:    strings.ToLower(record[gr.header["address"]]),
		Name:       record[gr.header["name"]],
		Decimals:   record[gr.header["decimals"]],
		Symbol:     record[gr.header["symbol"]],
		Source:     record[gr.header["source"]],
		Petname:    record[gr.header["petname"]],
		IsCustom:   record[gr.header["iscustom"]] == "true",
		IsPrefund:  record[gr.header["isprefund"]] == "true",
		IsContract: record[gr.header["iscontract"]] == "true",
		IsErc20:    record[gr.header["iserc20"]] == "true",
		IsErc721:   record[gr.header["iserc721"]] == "true",
		Deleted:    record[gr.header["deleted"]] == "true",
	}, nil
}

func NewNameReader(path string) (NameReader, error) {
	file, err := os.OpenFile(path, os.O_RDONLY, 0)
	if err != nil {
		return NameReader{}, err
	}

	reader := csv.NewReader(file)
	reader.Comma = '\t'
	if strings.HasSuffix(path, ".csv") {
		reader.Comma = ','
	}

	headerRow, err := reader.Read()
	if err != nil {
		return NameReader{}, err
	}
	header := map[string]int{}
	for index, columnName := range headerRow {
		header[columnName] = index
	}

	for _, required := range requiredColumns {
		_, ok := header[required]
		if !ok {
			err = fmt.Errorf(`required column "%s" missing in file %s`, required, path)
			return NameReader{}, err
		}
	}

	gr := NameReader{
		file:      file,
		header:    header,
		csvReader: *reader,
	}

	return gr, nil
}

func doSearch(name Name, terms []string, parts Parts) bool {
	if len(terms) == 0 {
		return true
	}

	cnt := 0
	searchStr := name.Name + "\t" + name.Symbol + "\t" + name.Address + "\t" + name.Petname + "\t" + name.Tags
	if parts&Expanded != 0 {
		searchStr += "\t" + name.Source
	}

	for _, term := range terms {
		if parts&MatchCase != 0 {
			if strings.Contains(searchStr, term) {
				cnt++
			}
		} else {
			x := strings.ToLower(searchStr)
			y := strings.ToLower(term)
			if strings.Contains(x, y) {
				cnt++
			}
		}
	}

	return len(terms) <= cnt
}

func getCustomTestNames(ret *NamesMap, terms []string, parts Parts) {
	for i := 1; i < 5; i++ {
		n := Name{
			Address: fmt.Sprintf("0x%040d", i),
			Name:    fmt.Sprintf("Account_%d", i),
			Tags:    "81-Custom",
			Source:  "Testing",
		}
		if i%2 == 0 {
			n.Decimals = fmt.Sprintf("%d", i)
			n.Symbol = fmt.Sprintf("AC_%d", i)
		}
		n.Petname = AddrToPetname(n.Address, "-")
		if doSearch(n, terms, parts) {
			(*ret)[types.HexToAddress(n.Address)] = n
		}
	}
}
