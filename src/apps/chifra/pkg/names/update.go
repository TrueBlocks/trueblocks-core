package names

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

func UpdateName(dbType DatabaseType, chain string, name *types.Name) (err error) {
	_ = chain
	switch dbType {
	case DatabaseCustom:
		return updateCustomName(name)
	case DatabaseRegular:
		return updateRegularName(name)
	default:
		logger.ShouldNotHappen("unknown database type")
	}
	return
}

func updateCustomName(name *types.Name) (err error) {
	// customCreateName locks for us
	// customNamesMutex.Lock()
	// defer customNamesMutex.Unlock()

	if _, ok := customNames[name.Address]; !ok {
		err = fmt.Errorf("no custom name for address %s", name.Address.Hex())
		return
	}

	return customCreateName("mainnet", name)
}

func updateRegularName(name *types.Name) (err error) {
	// regularCreateName locks for us
	// regularNamesMutex.Lock()
	// defer regularNamesMutex.Unlock()

	if _, ok := regularNames[name.Address]; !ok {
		err = fmt.Errorf("no name for address: %s", name.Address)
		return
	}

	return regularCreateName(name)
}
