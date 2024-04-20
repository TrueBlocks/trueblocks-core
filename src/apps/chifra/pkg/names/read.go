package names

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func ReadName(dbType DatabaseType, chain string, address base.Address) (name *types.Name) {
	switch dbType {
	case DatabaseCustom:
		return customReadName(address)
	case DatabaseRegular:
		return regularReadName(address)
	default:
		return nil
	}
}

func customReadName(address base.Address) (name *types.Name) {
	found, ok := loadedCustomNames[address]
	if ok {
		return &found
	}
	return nil
}

func regularReadName(address base.Address) (name *types.Name) {
	found, ok := loadedRegularNames[address]
	if ok {
		return &found
	}
	return nil
}
