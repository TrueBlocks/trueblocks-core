package names

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func RemoveName(dbType DatabaseType, chain string, address base.Address) (name *types.Name, err error) {
	switch dbType {
	case DatabaseCustom:
		return customRemoveName(chain, address)
	case DatabaseRegular:
		// return regularRemoveName(chain, address)
		logger.Fatal("should not happen ==> remove is not supported for regular database")
	default:
		logger.Fatal("should not happen ==> unknown database type")
	}
	return
}

func customRemoveName(chain string, address base.Address) (*types.Name, error) {
	name, exists := loadedCustomNames[address]
	if !exists {
		return nil, fmt.Errorf("cannot remove non-existant custom name for address %s", address.Hex())
	}

	db, err := openDatabaseFile(chain, DatabaseCustom, os.O_WRONLY|os.O_TRUNC)
	if err != nil {
		return nil, err
	}
	defer db.Close()

	loadedCustomNamesMutex.Lock()
	defer loadedCustomNamesMutex.Unlock()
	delete(loadedCustomNames, address)
	return &name, writeCustomNames(db)
}
