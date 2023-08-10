package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type AbiCache struct {
	Chain     string
	AbiMap    abi.FunctionSyncMap
	loadedMap abi.AddressSyncMap
	skipMap   abi.AddressSyncMap
}

func NewAbiCache(chain string, loadKnown bool) *AbiCache {
	ret := &AbiCache{
		Chain:     chain,
		AbiMap:    abi.FunctionSyncMap{},
		loadedMap: abi.AddressSyncMap{},
		skipMap:   abi.AddressSyncMap{},
	}

	if loadKnown {
		if err := abi.LoadKnownAbis(chain, &ret.AbiMap); err != nil {
			// report error, but continue processing
			logger.Error("error preloading known abis", "error", err)
		}
	}

	return ret
}
