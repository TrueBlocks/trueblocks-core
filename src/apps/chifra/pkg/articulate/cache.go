package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
)

type AbiCache struct {
	Conn      *rpc.Connection
	Chain     string
	AbiMap    abi.FunctionSyncMap
	loadedMap abi.AddressSyncMap
	skipMap   abi.AddressSyncMap
}

func NewAbiCache(conn *rpc.Connection, loadKnown bool) *AbiCache {
	ret := &AbiCache{
		Conn:      conn,
		Chain:     conn.Chain,
		AbiMap:    abi.FunctionSyncMap{},
		loadedMap: abi.AddressSyncMap{},
		skipMap:   abi.AddressSyncMap{},
	}

	if loadKnown {
		if err := abi.LoadKnownAbis(conn.Chain, &ret.AbiMap); err != nil {
			// report error, but continue processing
			logger.Error("error preloading known abis", "error", err)
		}
	}

	return ret
}
