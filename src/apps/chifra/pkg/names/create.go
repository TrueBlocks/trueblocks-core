package names

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func CreateName(dbType DatabaseType, chain string, name *types.SimpleName) (err error) {
	switch dbType {
	case DatabaseCustom:
		return customCreateName(chain, name)
	case DatabaseRegular:
		return regularCreateName(chain, name)
	default:
		logger.Fatal("unknown database type")
	}
	return
}

func customCreateName(chain string, name *types.SimpleName) (err error) {
	db, err := OpenDatabaseFile(chain, DatabaseCustom, os.O_WRONLY|os.O_TRUNC)
	if err != nil {
		return
	}
	defer db.Close()

	name.IsCustom = true

	return setCustomNameAndSave(db, name)
}

func regularCreateName(chain string, name *types.SimpleName) (err error) {
	loadedRegularNamesMutex.Lock()
	defer loadedRegularNamesMutex.Unlock()

	name.IsCustom = false
	loadedRegularNames[name.Address] = *name
	return
}
