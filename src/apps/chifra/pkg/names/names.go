package names

import (
	"encoding/binary"
	"encoding/csv"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"os"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/ethereum/go-ethereum/common"
)

// Name is a record in the names database
type Name struct {
	Tags        string `json:"tags"`
	Address     string `json:"address"`
	Name        string `json:"name"`
	Symbol      string `json:"symbol"`
	Source      string `json:"source"`
	Decimals    string `json:"decimals"`
	Description string `json:"description"`
	Deleted     bool   `json:"deleted"`
	IsCustom    bool   `json:"isCustom"`
	IsPrefund   bool   `json:"isPrefund"`
	IsContract  bool   `json:"isContract"`
	IsErc20     bool   `json:"isErc20"`
	IsErc721    bool   `json:"isErc721"`
}

func (n Name) String() string {
	ret, _ := json.MarshalIndent(n, "", "  ")
	return string(ret)
}

type NamesArray []Name

// NameOnDisc is a record in the names database when stored in the binary backing file
type NameOnDisc struct {
	Tags        [30 + 1]byte  `json:"-"`
	Address     [42 + 1]byte  `json:"-"`
	Name        [120 + 1]byte `json:"-"`
	Symbol      [30 + 1]byte  `json:"-"`
	Source      [180 + 1]byte `json:"-"`
	Description [255 + 1]byte `json:"-"`
	Decimals    uint16        `json:"-"`
	Flags       uint16        `json:"-"`
	Padding     byte          `json:"-"`
}

type NamesMap map[common.Address]Name

type NameOnDiscHeader struct {
	Magic   uint64
	Version uint64
	Count   uint64
	Padding [644]byte
}

func LoadNamesArray(chain string) (NamesArray, error) {
	names := NamesArray{}
	if namesMap, err := LoadNamesMap(chain); err != nil {
		return nil, err
	} else {
		for _, name := range namesMap {
			names = append(names, name)
		}
	}

	sort.Slice(names, func(i, j int) bool {
		return names[i].Address < names[j].Address
	})

	return names, nil
}

func LoadNamesMap(chain string) (NamesMap, error) {
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
				Tags:        justChars(v.Tags[:]),
				Address:     justChars(v.Address[:]),
				Name:        justChars(v.Name[:]),
				Symbol:      justChars(v.Symbol[:]),
				Source:      justChars(v.Source[:]),
				Description: justChars(v.Description[:]),
			}
			ret[common.HexToAddress(justChars(v.Address[:]))] = n
			// fmt.Println(n)
			// fmt.Println()
		}
		return ret, nil
	}

	ret := NamesMap{}
	namesPath := config.GetPathToChainConfig(chain) + "names.tab"
	gr, err := NewNameReader(namesPath)
	if err != nil {
		log.Fatal(err)
	}

	for {
		grant, err := gr.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			log.Fatal(err)
		}
		ret[common.HexToAddress(grant.Address)] = grant
	}

	return ret, nil
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
	"description",
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
		Tags:    record[gr.header["tags"]],
		Address: strings.ToLower(record[gr.header["address"]]),
		Name:    record[gr.header["name"]],
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
