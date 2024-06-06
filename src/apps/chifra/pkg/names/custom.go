package names

import (
	"errors"
	"fmt"
	"io"
	"os"
	"sort"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var loadedCustomNames map[base.Address]types.Name = map[base.Address]types.Name{}
var loadedCustomNamesMutex sync.Mutex

func loadCustomMap(chain string, terms []string, parts Parts, namesMap *map[base.Address]types.Name) (err error) {
	if len(loadedCustomNames) != 0 {
		// We have already loaded the data
		for _, name := range loadedCustomNames {
			if doSearch(&name, terms, parts) {
				(*namesMap)[name.Address] = name
			}
		}
		return
	}
	loadedCustomNamesMutex.Lock()
	defer loadedCustomNamesMutex.Unlock()

	db, err := openDatabaseFile(chain, DatabaseCustom, os.O_RDONLY)
	if err != nil {
		return err
	}
	defer db.Close()

	loadedCustomNames, err = unmarshallCustomNames(db, terms, parts, namesMap)
	if err != nil {
		return err
	}
	if parts&Testing != 0 {
		loadTestNames(terms, parts, &loadedCustomNames, namesMap)
	}
	return
}

func unmarshallCustomNames(source io.Reader, terms []string, parts Parts, namesMap *map[base.Address]types.Name) (customNames map[base.Address]types.Name, err error) {
	customNames = map[base.Address]types.Name{}

	var reader NameReader
	reader, err = NewNameReader(source)
	if err != nil {
		// Supress EOF, the file can be empty
		if errors.Is(err, io.EOF) {
			err = nil
		}
		return
	}

	for {
		var name types.Name
		name, err = reader.Read()
		if err == io.EOF {
			// Supress EOF, the file can be empty
			err = nil
			break
		}
		if err != nil {
			return
		}
		customNames[name.Address] = name
		if doSearch(&name, terms, parts) {
			(*namesMap)[name.Address] = name
		}
	}
	return
}

func loadTestNames(terms []string, parts Parts, all *map[base.Address]types.Name, namesMap *map[base.Address]types.Name) {
	for i := 1; i < 5; i++ {
		addressStr := fmt.Sprintf("0x%040d", i)
		num := fmt.Sprintf("%d", i)
		address := base.HexToAddress(addressStr)
		name := types.Name{
			Address:  address,
			Name:     "Account_" + num,
			Tags:     "81-Custom",
			Decimals: uint64(i),
			Symbol:   "SYM_" + num,
			Source:   "Testing",
			IsCustom: true,
		}
		if _, ok := (*all)[address]; !ok {
			(*all)[address] = name
		}
		if doSearch(&name, terms, parts) {
			(*namesMap)[name.Address] = name
		}
	}
}

func writeCustomNames(output *os.File) (err error) {
	if err = file.Lock(output); err != nil {
		return
	}
	defer func() {
		_ = file.Unlock(output)
	}()

	// We don't want to save test names even in test database
	var testAddresses map[string]bool = map[string]bool{
		"0x0000000000000000000000000000000000000001": true,
		"0x0000000000000000000000000000000000000002": true,
		"0x0000000000000000000000000000000000000003": true,
		"0x0000000000000000000000000000000000000004": true,
	}

	sorted := make([]types.Name, 0, len(loadedCustomNames))
	for _, name := range loadedCustomNames {
		sorted = append(sorted, name)
	}
	sort.Slice(sorted, func(i, j int) bool {
		return sorted[i].Address.Hex() < sorted[j].Address.Hex()
	})

	writer := NewNameWriter(output)
	for _, name := range sorted {
		if testAddresses[name.Address.Hex()] {
			continue
		}
		if err = writer.Write(&name); err != nil {
			return
		}
	}
	writer.Flush()
	return writer.Error()
}
