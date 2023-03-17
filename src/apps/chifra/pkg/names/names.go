package names

import (
	"encoding/csv"
	"fmt"
	"io"
	"io/fs"
	"os"
	"path"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type Parts int

// Parts is a bitfield that defines what parts of a name to return and other options
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

// SortBy is a bitfield that defines how to sort the names
const (
	SortByAddress SortBy = iota
	SortByName
	// SortBySymbol
	// SortBySource
	// SortByDecimals
	SortByTags
	// SortByPetname
)

// LoadNamesArray loads the names from the cache and returns an array of names
func LoadNamesArray(chain string, parts Parts, sortBy SortBy, terms []string) ([]types.SimpleName, error) {
	var names []types.SimpleName
	if namesMap, err := LoadNamesMap(chain, parts, terms); err != nil {
		return nil, err
	} else {
		for _, name := range namesMap {
			isTesting := parts&Testing != 0
			isIndiv := strings.Contains(name.Tags, "Individual")
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
			return names[i].Address.Hex() < names[j].Address.Hex()
		}
	})

	isTesting := parts&Testing != 0
	isTags := sortBy == SortByTags
	if isTesting && !isTags {
		names = names[:utils.Min(200, len(names))]
	}

	return names, nil
}

// LoadNamesMap loads the names from the cache and returns a map of names
func LoadNamesMap(chain string, parts Parts, terms []string) (map[base.Address]types.SimpleName, error) {
	namesMap := map[base.Address]types.SimpleName{}

	// Load the prefund names first...
	if parts&Prefund != 0 {
		prefundPath := filepath.Join(config.GetPathToChainConfig(chain), "allocs.csv")
		loadPrefundMap(chain, prefundPath, terms, parts, &namesMap)
	}

	if parts&Regular != 0 {
		namesPath := filepath.Join(config.GetPathToChainConfig(chain), "names.tab")
		loadRegularMap(chain, namesPath, terms, parts, &namesMap)
	}

	// Load the custom names (note that these may overwrite the prefund and regular names)
	if parts&Custom != 0 {
		// customPath := filepath.Join(config.GetPathToChainConfig(chain), "names_custom.tab")
		loadCustomMap(chain, terms, parts, &namesMap)
	}

	return namesMap, nil
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

func (gr *NameReader) Read() (types.SimpleName, error) {
	record, err := gr.csvReader.Read()
	if err == io.EOF {
		gr.file.Close()
	}
	if err != nil {
		return types.SimpleName{}, err
	}

	return types.SimpleName{
		Tags:       record[gr.header["tags"]],
		Address:    base.HexToAddress(strings.ToLower(record[gr.header["address"]])),
		Name:       record[gr.header["name"]],
		Decimals:   globals.ToUint64(record[gr.header["decimals"]]),
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

type NameReaderMode int

const (
	NameReaderTab NameReaderMode = iota
	NameReaderComma
)

func NewNameReader(source io.Reader, mode NameReaderMode) (NameReader, error) {
	// file, err := os.OpenFile(path, os.O_RDONLY, 0)
	// if err != nil {
	// 	return NameReader{}, err
	// }

	reader := csv.NewReader(source)
	reader.Comma = '\t'
	if mode == NameReaderComma {
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
			err = fmt.Errorf(`required column "%s" missing`, required) //, path)
			return NameReader{}, err
		}
	}

	r := NameReader{
		// file:      file,
		header:    header,
		csvReader: *reader,
	}

	return r, nil
}

type DatabaseFile string

const (
	DatabaseRegular DatabaseFile = "names.tab"
	DatabaseCustom  DatabaseFile = "names_custom.tab"
	DatabasePrefund DatabaseFile = "allocs.csv"
)

func OpenDatabaseFile(chain string, kind DatabaseFile, openFlag int) (*os.File, error) {
	filePath := filepath.Join(config.GetPathToChainConfig(chain), string(kind))
	var permissions fs.FileMode = 0666

	if kind == DatabaseCustom && os.Getenv("TEST_MODE") == "true" {
		// Create temp database, just for tests
		if err := os.MkdirAll(path.Join(os.TempDir(), "trueblocks"), 0777); err != nil {
			return nil, err
		}

		filePath = path.Join(os.TempDir(), "trueblocks", "names_custom.tab")
		openFlag |= os.O_CREATE
		permissions = 0777
	}

	return os.OpenFile(
		filePath,
		openFlag,
		permissions,
	)
}

/*
// NameOnDiscHeader is the header of the names database when stored in the binary backing file
type NameOnDiscHeader struct {
	Magic   uint64    // 8 bytes
	Version uint64    // + 8 bytes = 16 bytes
	Count   uint64    // + 8 bytes = 24 bytes
	Padding [428]byte // 452 - 24 = 428 bytes
}

// NameOnDisc is a record in the names database when stored in the binary backing file
type NameOnDisc struct {
	Tags     [30 + 1]byte  `json:"-"` // 31 bytes
	Address  [42 + 1]byte  `json:"-"` // + 43 bytes = 74 bytes
	Name     [120 + 1]byte `json:"-"` // + 121 bytes = 195 bytes
	Symbol   [30 + 1]byte  `json:"-"` // + 31 bytes = 226 bytes
	Source   [180 + 1]byte `json:"-"` // + 181 bytes = 407 bytes
	Petname  [40 + 1]byte  `json:"-"` // + 41 bytes = 448 bytes
	Decimals uint16        `json:"-"` // + 2 bytes = 450 bytes
	Flags    uint16        `json:"-"` // + 2 bytes = 452 bytes
}

// Bitflags for the Flags field
const (
	IsCustom   uint16 = 0x1
	IsPrefund  uint16 = 0x2
	IsContract uint16 = 0x4
	IsErc20    uint16 = 0x8
	IsErc721   uint16 = 0x10
	IsDeleted  uint16 = 0x20
)

// asString converts the byte array (not zero-terminated) to a string
func asString(which string, b []byte) string {
	ret := ""
	for _, rVal := range string(b) {
		if rVal == 0 {
			return ret
		}
		ret += string(rVal)
	}
	return ret
}

	// binPath := config.GetPathToCache(chain) + "names/names.bin"
	// namesPath := filepath.Join(config.GetPathToChainConfig(chain), "names.tab")
	// customPath := filepath.Join(config.GetPathToChainConfig(chain), "names_custom.tab")
	// Load the names from the binary file (note that these may overwrite the prefund names)
	if parts&Regular != 0 {
		// enabled := false // os.Getenv("FAST") == "true" // TODO: this isn't right
		// if enabled && file.FileExists(binPath) {
		// 	file, _ := os.OpenFile(binPath, os.O_RDONLY, 0)
		// 	defer file.Close()

		// 	header := NameOnDiscHeader{}
		// 	err := binary.Read(file, binary.LittleEndian, &header)
		// 	if err != nil {
		// 		return nil, err
		// 	}

		// 	for i := uint64(0); i < header.Count; i++ {
		// 		v := NameOnDisc{}
		// 		binary.Read(file, binary.LittleEndian, &v)
		// 		n := Name{
		// 			Tags:       asString("tags", v.Tags[:]),
		// 			Address:    asString("address", v.Address[:]),
		// 			Name:       asString("name", v.Name[:]),
		// 			Symbol:     asString("symbol", v.Symbol[:]),
		// 			Decimals:   fmt.Sprintf("%d", v.Decimals),
		// 			Source:     asString("source", v.Source[:]),
		// 			Petname:    asString("petname", v.Petname[:]),
		// 			IsCustom:   v.Flags&IsCustom != 0,
		// 			IsPrefund:  v.Flags&IsPrefund != 0,
		// 			IsContract: v.Flags&IsContract != 0,
		// 			IsErc20:    v.Flags&IsErc20 != 0,
		// 			IsErc721:   v.Flags&IsErc721 != 0,
		// 			Deleted:    v.Flags&IsDeleted != 0,
		// 		}
		// 		if !n.IsCustom {
		// 			if doSearch(n, terms, parts) {
		// 				ret[base.HexToAddress(n.Address)] = n
		// 			}
		// 		}
		// 	}
		// } else {
		namesPath := filepath.Join(config.GetPathToChainConfig(chain), "names.tab")
		loadRegularMap(chain, namesPath, terms, parts, &ret)
		// }
	}
*/
