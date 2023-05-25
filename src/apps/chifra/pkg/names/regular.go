package names

import (
	"fmt"
	"io"
	"os"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: Test if there's a performance differnce between using an array here (which would work just as well) and a map
var loadedRegularNames map[base.Address]types.SimpleName = map[base.Address]types.SimpleName{}
var loadedRegularNamesMutex sync.Mutex

// loadRegularMap loads the regular names from the cache
func loadRegularMap(chain string, thePath string, terms []string, parts Parts, ret *map[base.Address]types.SimpleName) error {
	if len(loadedRegularNames) != 0 {
		// We have already loaded the data
		for _, name := range loadedRegularNames {
			if doSearch(&name, terms, parts) {
				(*ret)[name.Address] = name
			}
		}
		return nil
	}
	loadedRegularNamesMutex.Lock()
	defer loadedRegularNamesMutex.Unlock()

	db, err := OpenDatabaseFile(chain, DatabaseRegular, os.O_RDONLY)
	if err != nil {
		return err
	}
	defer db.Close()

	reader, err := NewNameReader(db, NameReaderTab)
	if err != nil {
		return err
	}

	for {
		n, err := reader.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			logger.Fatal(err)
		}
		loadedRegularNames[n.Address] = n
		if doSearch(&n, terms, parts) {
			(*ret)[n.Address] = n
		}
	}

	return nil
}

func WriteRegularNames(chain string, overrideDest Database) (err error) {
	database := DatabaseRegular
	if overrideDest != "" {
		database = overrideDest
	}
	return WriteDatabase(
		chain,
		Regular,
		database,
		loadedRegularNames,
	)
}

func CreateRegularName(name *types.SimpleName) (err error) {
	loadedRegularNamesMutex.Lock()
	defer loadedRegularNamesMutex.Unlock()

	name.IsCustom = false
	loadedRegularNames[name.Address] = *name
	return
}

func ReadRegularName(address base.Address) (name *types.SimpleName) {
	found, ok := loadedRegularNames[address]
	if ok {
		return &found
	}
	return nil
}

func UpdateRegularName(name *types.SimpleName) (err error) {
	if _, ok := loadedRegularNames[name.Address]; !ok {
		err = fmt.Errorf("no name for address: %s", name.Address)
		return
	}

	return CreateRegularName(name)
}
