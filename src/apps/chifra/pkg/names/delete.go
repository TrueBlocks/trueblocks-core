package names

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func SetDeleted(dbType DatabaseType, chain string, address base.Address, deleted bool) (name *types.Name, err error) {
	switch dbType {
	case DatabaseCustom:
		return customSetDeleted(chain, address, deleted)
	case DatabaseRegular:
		// return regularSetDeleted(chain, address, deleted)
		logger.Fatal("should not happen ==> SetDeleted is not supported for regular database")
	default:
		logger.Fatal("should not happen ==> unknown database type")
	}
	return
}

func customSetDeleted(chain string, address base.Address, deleted bool) (name *types.Name, err error) {
	db, err := openDatabaseFile(chain, DatabaseCustom, os.O_WRONLY|os.O_TRUNC)
	if err != nil {
		return
	}
	defer db.Close()

	return changeDeleted(db, address, deleted)
}

func changeDeleted(output *os.File, address base.Address, deleted bool) (*types.Name, error) {
	name, ok := loadedCustomNames[address]
	if !ok {
		return nil, fmt.Errorf("no custom name for address %s", address.Hex())
	}

	name.Deleted = deleted
	name.IsCustom = true
	loadedCustomNamesMutex.Lock()
	defer loadedCustomNamesMutex.Unlock()
	loadedCustomNames[name.Address] = name
	return &name, writeCustomNames(output)
}
