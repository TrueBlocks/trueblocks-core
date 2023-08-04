package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type AbiCache struct {
	abiMap    abi.AbiInterfaceMap
	loadedMap map[base.Address]bool
	skipMap   map[base.Address]bool
}

func NewAbiCache(chain string, loadKnown bool) *AbiCache {
	ret := &AbiCache{
		abiMap:    make(abi.AbiInterfaceMap),
		loadedMap: make(map[base.Address]bool),
		skipMap:   make(map[base.Address]bool),
	}

	if loadKnown {
		if err := abi.LoadKnownAbis(chain, ret.abiMap); err != nil {
			// report error, but continue processing
			logger.Error("error preloading known abis", "error", err)
		}
	}

	return ret
}
