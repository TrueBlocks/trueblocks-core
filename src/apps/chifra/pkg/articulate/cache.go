package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc"
)

type AbiCache struct {
	Conn      *rpc.Connection
	Chain     string
	AbiMap    abi.SelectorSyncMap
	loadedMap abi.AddressSyncMap
	skipMap   abi.AddressSyncMap
}

func NewAbiCache(conn *rpc.Connection, loadKnown bool) *AbiCache {
	ret := &AbiCache{
		Conn:      conn,
		Chain:     conn.Chain,
		AbiMap:    abi.SelectorSyncMap{},
		loadedMap: abi.AddressSyncMap{},
		skipMap:   abi.AddressSyncMap{},
	}

	if loadKnown {
		if err := ret.AbiMap.LoadKnownAbis(conn.Chain); err != nil {
			// report error, but continue processing
			logger.Error("error preloading known abis", "error", err)
		}
	}

	return ret
}
