package names

import (
	"errors"
	"fmt"
	"io"
	"os"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var loadedCustomNames map[base.Address]types.SimpleName = map[base.Address]types.SimpleName{}
var loadedCustomNamesMutex sync.Mutex

// We don't want to save test names even in test database
var testAddresses map[string]bool = map[string]bool{
	"0x0000000000000000000000000000000000000001": true,
	"0x0000000000000000000000000000000000000002": true,
	"0x0000000000000000000000000000000000000003": true,
	"0x0000000000000000000000000000000000000004": true,
}

func loadCustomMap(chain string, terms []string, parts Parts, destination *map[base.Address]types.SimpleName) (err error) {
	if len(loadedCustomNames) != 0 {
		// We have already loaded the data
		for _, name := range loadedCustomNames {
			if doSearch(&name, terms, parts) {
				(*destination)[name.Address] = name
			}
		}
		return
	}
	loadedCustomNamesMutex.Lock()
	defer loadedCustomNamesMutex.Unlock()

	db, err := OpenDatabaseFile(chain, DatabaseCustom, os.O_RDONLY)
	if err != nil {
		return err
	}
	defer db.Close()

	loadedCustomNames, err = unmarshallCustomNames(db, terms, parts, destination)
	if err != nil {
		return err
	}
	if parts&Testing != 0 {
		loadTestNames(terms, parts, &loadedCustomNames, destination)
	}
	return
}

func unmarshallCustomNames(source io.Reader, terms []string, parts Parts, destination *map[base.Address]types.SimpleName) (customNames map[base.Address]types.SimpleName, err error) {
	customNames = map[base.Address]types.SimpleName{}

	var reader NameReader
	reader, err = NewNameReader(source, NameReaderTab)
	if err != nil {
		// Supress EOF, the file can be empty
		if errors.Is(err, io.EOF) {
			err = nil
		}
		return
	}

	for {
		var name types.SimpleName
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
			(*destination)[name.Address] = name
		}
	}
	return
}

func loadTestNames(terms []string, parts Parts, all *map[base.Address]types.SimpleName, destination *map[base.Address]types.SimpleName) {
	for i := 1; i < 5; i++ {
		addressStr := fmt.Sprintf("0x%040d", i)
		num := fmt.Sprintf("%d", i)
		address := base.HexToAddress(addressStr)
		name := types.SimpleName{
			Address:  address,
			Name:     "Account_" + num,
			Tags:     "81-Custom",
			Decimals: uint64(i),
			Symbol:   "SYM_" + num,
			Source:   "Testing",
			Petname:  base.AddrToPetname(addressStr, "-"),
			IsCustom: true,
		}
		if _, ok := (*all)[address]; !ok {
			(*all)[address] = name
		}
		if doSearch(&name, terms, parts) {
			(*destination)[name.Address] = name
		}
	}
}

func CreateCustomName(chain string, name *types.SimpleName) (err error) {
	db, err := OpenDatabaseFile(chain, DatabaseCustom, os.O_WRONLY|os.O_TRUNC)
	if err != nil {
		return
	}
	defer db.Close()
	name.IsCustom = true
	return setCustomNameAndSave(db, name)
}

func ReadCustomName(address base.Address) (name *types.SimpleName) {
	found, ok := loadedCustomNames[address]
	if ok {
		return &found
	}
	return nil
}

func UpdateCustomName(name *types.SimpleName) (err error) {
	loadedCustomNamesMutex.Lock()
	defer loadedCustomNamesMutex.Unlock()

	name.IsCustom = true
	if _, ok := loadedCustomNames[name.Address]; !ok {
		err = fmt.Errorf("no custom name for address %s", name.Address.Hex())
		return
	}

	loadedCustomNames[name.Address] = *name
	return
}

func UpdateAndSaveCustomName(chain string, name *types.SimpleName) (result *types.SimpleName, err error) {
	db, err := OpenDatabaseFile(chain, DatabaseCustom, os.O_WRONLY|os.O_TRUNC)
	if err != nil {
		return
	}
	defer db.Close()
	return setIfExists(db, name)
}

func setIfExists(output *os.File, name *types.SimpleName) (result *types.SimpleName, err error) {
	if _, ok := loadedCustomNames[name.Address]; !ok {
		return nil, fmt.Errorf("no custom name for address %s", name.Address.Hex())
	}

	return name, setCustomNameAndSave(output, name)
}

func ChangeCustomNameDeletedFlag(chain string, address base.Address, deleted bool) (name *types.SimpleName, err error) {
	db, err := OpenDatabaseFile(chain, DatabaseCustom, os.O_WRONLY|os.O_TRUNC)
	if err != nil {
		return
	}
	defer db.Close()

	return changeDeleted(db, address, deleted)
}

func changeDeleted(output *os.File, address base.Address, deleted bool) (name *types.SimpleName, err error) {
	if found, ok := loadedCustomNames[address]; ok {
		found.Deleted = deleted
		name = &found
	} else {
		return nil, fmt.Errorf("no custom name for address %s", address.Hex())
	}

	err = setCustomNameAndSave(output, name)
	return
}

func RemoveCustomName(chain string, address base.Address) (name *types.SimpleName, err error) {
	db, err := OpenDatabaseFile(chain, DatabaseCustom, os.O_WRONLY|os.O_TRUNC)
	if err != nil {
		return
	}
	defer db.Close()

	return removeIfExists(db, address)
}

func removeIfExists(output *os.File, address base.Address) (name *types.SimpleName, err error) {
	found, ok := loadedCustomNames[address]
	if !ok {
		return nil, fmt.Errorf("no custom name for address %s", address.Hex())
	}
	// This is commented out to match the behaviour of C++ code
	// if !found.Deleted {
	// 	return nil, fmt.Errorf("name for %s is not marked for deletion", address.Hex())
	// }
	loadedCustomNamesMutex.Lock()
	defer loadedCustomNamesMutex.Unlock()

	delete(loadedCustomNames, address)
	return &found, writeCustomNames(output)
}

func setCustomNameAndSave(output *os.File, name *types.SimpleName) (err error) {
	loadedCustomNamesMutex.Lock()
	defer loadedCustomNamesMutex.Unlock()

	// Make sure we always set correct value of the flag
	name.IsCustom = true
	loadedCustomNames[name.Address] = *name
	return writeCustomNames(output)
}

func writeCustomNames(output *os.File) (err error) {
	if err = file.Lock(output); err != nil {
		return
	}
	defer func() {
		_ = file.Unlock(output)
	}()

	writer := NewNameWriter(output)
	for _, name := range loadedCustomNames {
		if os.Getenv("TEST_MODE") == "true" {
			// Do not save test addresses
			if testAddresses[name.Address.Hex()] {
				continue
			}
		}
		if err = writer.Write(&name); err != nil {
			return
		}
	}
	writer.Flush()

	return writer.Error()
}

func WriteCustomNames(chain string, overrideDest Database) (err error) {
	database := DatabaseCustom
	if overrideDest != "" {
		database = overrideDest
	}
	return WriteDatabase(
		chain,
		Custom,
		database,
		loadedCustomNames,
	)
}
