package names

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func UpdateName(dbType DatabaseType, chain string, name *types.Name) (err error) {
	switch dbType {
	case DatabaseCustom:
		return updateCustomName(chain, name)
	case DatabaseRegular:
		return updateRegularName(chain, name)
	default:
		logger.Fatal("should not happen ==> unknown database type")
	}
	return
}

func updateCustomName(chain string, name *types.Name) (err error) {
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

func updateRegularName(chain string, name *types.Name) (err error) {
	if _, ok := loadedRegularNames[name.Address]; !ok {
		err = fmt.Errorf("no name for address: %s", name.Address)
		return
	}

	return regularCreateName(chain, name)
}
