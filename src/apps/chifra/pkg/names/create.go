package names

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func CreateName(dbType DatabaseType, chain string, name *types.Name) (err error) {
	switch dbType {
	case DatabaseCustom:
		return customCreateName(chain, name)
	case DatabaseRegular:
		return regularCreateName(name)
	default:
		logger.Fatal("should not happen ==> unknown database type")
	}
	return
}

func customCreateName(chain string, name *types.Name) (err error) {
	db, err := openDatabaseForEdit(chain, DatabaseCustom)
	if err != nil {
		return
	}
	defer db.Close()

	name.IsCustom = true
	loadedCustomNamesMutex.Lock()
	defer loadedCustomNamesMutex.Unlock()
	loadedCustomNames[name.Address] = *name
	return writeCustomNames(db)
}

func regularCreateName(name *types.Name) (err error) {
	loadedRegularNamesMutex.Lock()
	defer loadedRegularNamesMutex.Unlock()

	name.IsCustom = false
	loadedRegularNames[name.Address] = *name
	return
}
