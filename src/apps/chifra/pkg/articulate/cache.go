package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type AbiCache struct {
	abiMap    abi.AbiInterfaceMap
	loadedMap map[base.Address]bool
	skipMap   map[base.Address]bool
}

func NewAbiCache() *AbiCache {
	return &AbiCache{
		abiMap:    make(abi.AbiInterfaceMap),
		loadedMap: make(map[base.Address]bool),
		skipMap:   make(map[base.Address]bool),
	}
}
