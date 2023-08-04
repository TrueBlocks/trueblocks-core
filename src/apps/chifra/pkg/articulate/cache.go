package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
)

type AbiCache struct {
	abiMap    abi.FunctionSyncMap
	loadedMap abi.AddressSyncMap
	skipMap   abi.AddressSyncMap
}

func NewAbiCache() *AbiCache {
	return &AbiCache{
		abiMap:    abi.FunctionSyncMap{},
		loadedMap: abi.AddressSyncMap{},
		skipMap:   abi.AddressSyncMap{},
	}
}
