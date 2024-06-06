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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/prefunds"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type Parts int

// Parts is a bitfield that defines what parts of a name to return and other options
const (
	None Parts = (1 << iota)
	Regular
	Custom
	Prefund
	Baddress
	Testing
	MatchCase
	Expanded
	Tags
)

type SortBy int

// SortBy is a bitfield that defines how to sort the names
const (
	SortByAddress SortBy = iota
	SortByTags
)

// LoadNamesArray loads the names from the cache and returns an array of names
func LoadNamesArray(chain string, parts Parts, sortBy SortBy, terms []string) ([]types.Name, error) {
	var names []types.Name
	if namesMap, err := LoadNamesMap(chain, parts, terms); err != nil {
		return nil, err
	} else {
		for _, name := range namesMap {
			// Custom names with Individual tag or tags under 30 are private during testing
			isTesting := parts&Testing != 0
			isPrivate := strings.Contains(name.Tags, "Individual") || (name.IsCustom && name.Tags < "3")
			if !isTesting || !isPrivate {
				names = append(names, name)
			}
		}
	}

	sort.Slice(names, func(i, j int) bool {
		switch sortBy {
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
		names = names[:base.Min(200, len(names))]
	}

	return names, nil
}

// LoadNamesMap loads the names from the cache and returns a map of names
func LoadNamesMap(chain string, parts Parts, terms []string) (map[base.Address]types.Name, error) {
	namesMap := map[base.Address]types.Name{}

	// Load the prefund names first...
	if parts&Prefund != 0 {
		prefundPath := prefunds.GetPrefundPath(chain)
		if prefundMap, err := prefunds.LoadPrefundMap(chain, prefundPath); err != nil {
			return namesMap, err
		} else {
			for k, v := range *prefundMap {
				if doSearch(&v, terms, parts) {
					namesMap[k] = v
				}
			}
		}
	}

	if parts&Regular != 0 {
		_ = loadRegularMap(chain, terms, parts, &namesMap)
	}

	// Load the custom names (note that these may overwrite the prefund and regular names)
	if parts&Custom != 0 {
		_ = loadCustomMap(chain, terms, parts, &namesMap)
	}

	return namesMap, nil
}

// ClearCache removes names that are cached in-memory
func ClearCache() {
	loadedRegularNamesMutex.Lock()
	defer loadedRegularNamesMutex.Unlock()

	loadedCustomNamesMutex.Lock()
	defer loadedCustomNamesMutex.Unlock()

	loadedRegularNames = make(map[base.Address]types.Name)
	loadedCustomNames = make(map[base.Address]types.Name)
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

func (gr *NameReader) Read() (types.Name, error) {
	record, err := gr.csvReader.Read()
	if err == io.EOF {
		gr.file.Close()
	}
	if err != nil {
		return types.Name{}, err
	}

	return types.Name{
		Tags:       record[gr.header["tags"]],
		Address:    base.HexToAddress(strings.ToLower(record[gr.header["address"]])),
		Name:       record[gr.header["name"]],
		Decimals:   base.MustParseUint64(record[gr.header["decimals"]]),
		Symbol:     record[gr.header["symbol"]],
		Source:     record[gr.header["source"]],
		Petname:    record[gr.header["petname"]],
		Deleted:    record[gr.header["deleted"]] == "true",
		IsCustom:   record[gr.header["isCustom"]] == "true",
		IsPrefund:  record[gr.header["isPrefund"]] == "true",
		IsContract: record[gr.header["isContract"]] == "true",
		IsErc20:    record[gr.header["isErc20"]] == "true",
		IsErc721:   record[gr.header["isErc721"]] == "true",
	}, nil
}

func NewNameReader(source io.Reader) (NameReader, error) {
	reader := csv.NewReader(source)
	reader.Comma = '\t'

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

type DatabaseType string

const (
	DatabaseRegular DatabaseType = "names.tab"
	DatabaseCustom  DatabaseType = "names_custom.tab"
	DatabasePrefund DatabaseType = "allocs.csv"
	DatabaseDryRun  DatabaseType = "<dryrun>"
)

func openDatabaseFile(chain string, kind DatabaseType, openFlag int) (*os.File, error) {
	if kind == DatabaseDryRun {
		return os.Stdout, nil
	}

	filePath := filepath.Join(config.MustGetPathToChainConfig(chain), string(kind))
	var permissions fs.FileMode = 0666

	if kind == DatabaseCustom && os.Getenv("TEST_MODE") == "true" {
		// Create temp database, just for tests. On Mac, the permissions must be set to 0777
		if err := os.MkdirAll(path.Join(os.TempDir(), "trueblocks"), 0777); err != nil {
			return nil, err
		}

		filePath = path.Join(os.TempDir(), "trueblocks", "names_custom.tab")
		openFlag |= os.O_CREATE
		// On Mac, the permissions must be set to 0777
		permissions = 0777
	}

	return os.OpenFile(
		filePath,
		openFlag,
		permissions,
	)
}
