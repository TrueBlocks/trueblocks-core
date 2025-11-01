package names

import (
	"encoding/csv"
	"fmt"
	"io"
	"io/fs"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/prefunds"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

// LoadNamesMap loads the names from the cache and returns a map of names
func LoadNamesMap(chain string, parts types.Parts, terms []string) (map[base.Address]types.Name, error) {
	namesMap := map[base.Address]types.Name{}

	// Load the prefund names first...
	if parts&types.Prefund != 0 {
		_ = loadPrefundMap(chain, terms, parts, &namesMap)
	}

	// Overlay them with regular names if present
	if parts&types.Regular != 0 {
		_ = loadRegularMap(chain, terms, parts, &namesMap)
	}

	// Add in the baddresses
	if parts&types.Baddress != 0 {
		_ = loadKnownBadresses(chain, terms, parts, &namesMap)
	}

	// Load the custom names (note that these may overwrite the prefund and regular names)
	if parts&types.Custom != 0 {
		_ = loadCustomMap(chain, terms, parts, &namesMap)
	}

	return namesMap, nil
}

// loadPrefundMap loads the prefund names from the cache
func loadPrefundMap(chain string, terms []string, parts types.Parts, ret *map[base.Address]types.Name) error {
	prefundPath := prefunds.GetPrefundPath(chain)
	if prefundMap, err := prefunds.LoadPrefundMap(chain, prefundPath); err != nil {
		return err
	} else {
		for k, v := range *prefundMap {
			if doSearch(&v, terms, parts) {
				v.Parts = types.Prefund
				if existing, ok := (*ret)[k]; ok {
					v.Parts |= existing.Parts
				}
				(*ret)[k] = v
			}
		}
	}
	return nil
}

// ClearCache removes names that are cached in-memory
func ClearCache() {
	regularNamesMutex.Lock()
	defer regularNamesMutex.Unlock()

	customNamesMutex.Lock()
	defer customNamesMutex.Unlock()

	regularNames = make(map[base.Address]types.Name)
	customNames = make(map[base.Address]types.Name)
}

var requiredColumns = []string{
	"tags",
	"address",
	"name",
	"symbol",
	"source",
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
	DatabaseDryRun  DatabaseType = "<dryrun>"
)

func getDatabasePath(chain string, dbType DatabaseType) string {
	if dbType == DatabaseDryRun {
		return ""
	}
	filePath := filepath.Join(config.MustGetPathToChainConfig(chain), string(dbType))
	return filePath
}

func openDatabaseForEdit(chain string, dbType DatabaseType) (*os.File, error) {
	// This opens the file destructively, truncating it to zero length.
	// Make sure to make a backup before calling this function.
	return openDatabaseFile(chain, dbType, os.O_WRONLY|os.O_TRUNC)
}

func openDatabaseForRead(chain string, dbType DatabaseType) (*os.File, error) {
	return openDatabaseFile(chain, dbType, os.O_RDONLY)
}

func openDatabaseFile(chain string, dbType DatabaseType, openFlag int) (*os.File, error) {
	if dbType == DatabaseDryRun {
		return os.Stdout, nil
	}

	filePath := filepath.Join(config.MustGetPathToChainConfig(chain), string(dbType))
	var permissions fs.FileMode = 0666

	if dbType == DatabaseCustom && base.IsTestMode() {
		// Create temp database, just for tests. On Mac, the permissions must be set to 0777
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
