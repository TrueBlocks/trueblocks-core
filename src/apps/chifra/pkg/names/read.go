package names

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

func ReadName(dbType DatabaseType, chain string, address base.Address) (name *types.Name) {
	_ = chain
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
	found, ok := customNames[address]
	if ok {
		return &found
	}
	return nil
}

func regularReadName(address base.Address) (name *types.Name) {
	found, ok := regularNames[address]
	if ok {
		return &found
	}
	return nil
}
