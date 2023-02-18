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
	Tags     [30 + 1]byte  `json:"-"`
	Address  [42 + 1]byte  `json:"-"`
	Name     [120 + 1]byte `json:"-"`
	Symbol   [30 + 1]byte  `json:"-"`
	Source   [180 + 1]byte `json:"-"`
	Petname  [40 + 1]byte  `json:"-"`
	Decimals uint16        `json:"-"`
	Flags    uint16        `json:"-"`
	Padding  byte          `json:"-"`
}

type NamesMap map[types.Address]Name

type NameOnDiscHeader struct {
	Magic   uint64
	Version uint64
	Count   uint64
	Padding [644]byte
}

type Component int

const (
	None    Component = 0x0
	Regular Component = 0x1
	Custom  Component = 0x2
	Prefund Component = 0x4
	Testing Component = 0x8
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

func LoadNamesArray(chain string, components Component, sortBy SortBy, terms []string) (NamesArray, error) {
	names := NamesArray{}
	if namesMap, err := LoadNamesMap(chain, components, terms); err != nil {
		return nil, err
	} else {
		for _, name := range namesMap {
			isTesting := components&Testing != 0
			if !isTesting || !strings.Contains(name.Tags, "Individual") {
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

	return names, nil
}

func LoadNamesMap(chain string, components Component, terms []string) (NamesMap, error) {
	binPath := config.GetPathToCache(chain) + "names/names.bin"
	// TODO: Use the names cache if it's present
	if false && file.FileExists(binPath) {
		file, _ := os.OpenFile(binPath, os.O_RDONLY, 0)
		defer file.Close()

		thing := NameOnDiscHeader{}
		binary.Read(file, binary.LittleEndian, &thing.Magic)
		binary.Read(file, binary.LittleEndian, &thing.Version)
		binary.Read(file, binary.LittleEndian, &thing.Count)
		binary.Read(file, binary.LittleEndian, &thing.Padding)
		// fmt.Println(thing)

		ret := NamesMap{}
		for i := uint64(0); i < thing.Count; i++ {
			v := NameOnDisc{}
			binary.Read(file, binary.LittleEndian, &v)
			n := Name{
				Tags:     justChars(v.Tags[:]),
				Address:  justChars(v.Address[:]),
				Name:     justChars(v.Name[:]),
				Symbol:   justChars(v.Symbol[:]),
				Decimals: justChars([]byte(fmt.Sprintf("%d", v.Decimals))),
				Source:   justChars(v.Source[:]),
				Petname:  justChars(v.Petname[:]),
			}
			if doSearch(n, terms) {
				ret[types.HexToAddress(justChars(v.Address[:]))] = n
			}
			// fmt.Println(n)
			// fmt.Println()
		}
		return ret, nil
	}

	ret := NamesMap{}
	if components&Regular != 0 {
		nameMapFromFile(chain, &ret, "names.tab", terms)
	}

	if components&Custom != 0 {
		nameMapFromFile(chain, &ret, "names_custom.tab", terms)
	}

	return ret, nil
}

func nameMapFromFile(chain string, ret *NamesMap, filename string, terms []string) {
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
		if doSearch(n, terms) {
			(*ret)[types.HexToAddress(n.Address)] = n
		}
	}
}

func justChars(b []byte) string {
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

	// isActive := record[gr.header["active"]] == "true"
	// isCore := record[gr.header["core"]] == "true"
	// isValid := validate.IsValidAddress(record[gr.header["address"]]) && !validate.IsZeroAddress(record[gr.header["address"]])
	return Name{
		Tags:     record[gr.header["tags"]],
		Address:  strings.ToLower(record[gr.header["address"]]),
		Name:     record[gr.header["name"]],
		Decimals: record[gr.header["decimals"]],
		Symbol:   record[gr.header["symbol"]],
		Petname:  record[gr.header["petname"]],
		// IsActive: isActive,
		// IsCore:   isCore,
		// IsValid:  isValid,
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

func doSearch(name Name, terms []string) bool {
	if len(terms) == 0 {
		return true
	}

	cnt := 0
	for _, term := range terms {
		if strings.Contains(strings.ToLower(name.Name), strings.ToLower(term)) {
			cnt++
		}
		if strings.Contains(strings.ToLower(name.Symbol), strings.ToLower(term)) {
			cnt++
		}
		if strings.Contains(strings.ToLower(name.Address), strings.ToLower(term)) {
			cnt++
		}
		if strings.Contains(strings.ToLower(name.Petname), strings.ToLower(term)) {
			cnt++
		}
		if strings.Contains(strings.ToLower(name.Tags), strings.ToLower(term)) {
			cnt++
		}
		if strings.Contains(strings.ToLower(name.Source), strings.ToLower(term)) {
			cnt++
		}
	}
	// if cnt > 0 {
	// fmt.Println(name.Address, name.Name, terms, len(terms), cnt, len(terms) == cnt)
	// }
	return len(terms) <= cnt
}
