package names

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func UpdateName(dbType DatabaseType, chain string, name *types.Name) (err error) {
	switch dbType {
	case DatabaseCustom:
		return updateCustomName(name)
	case DatabaseRegular:
		return updateRegularName(name)
	default:
		logger.Fatal("should not happen ==> unknown database type")
	}
	return
}

func updateCustomName(name *types.Name) (err error) {
	customNamesMutex.Lock()
	defer customNamesMutex.Unlock()

	name.IsCustom = true
	if _, ok := customNames[name.Address]; !ok {
		err = fmt.Errorf("no custom name for address %s", name.Address.Hex())
		return
	}

	customNames[name.Address] = *name
	return
}

func updateRegularName(name *types.Name) (err error) {
	if _, ok := regularNames[name.Address]; !ok {
		err = fmt.Errorf("no name for address: %s", name.Address)
		return
	}

	return regularCreateName(name)
}
